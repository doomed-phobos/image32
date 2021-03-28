#include "libbmp.h"

#include "../image_priv.h"
#include "../file/file.h"
#include "../string.h"

#include <cstdint>

#define ABS(value) value < 0 ? -value : value
#define WININFOHEADERSIZE 40
#define MAXBMPSIZE 1024*1024*128
#define BMP_8BITS_MAX_PALETTE_SIZE 256

#define BMP_FLAGS_WIN            (1 << 0)
#define BMP_FLAGS_PALETTE_TO_RGB (1 << 1)

struct BMPFILEHEADER
{
   uint16_t bfType;
   uint32_t bfSize;
   uint16_t bfReserved1;
   uint16_t bfReserved2;
   uint32_t bfOffBits;
};

struct BMPINFOHEADER
{
   uint32_t biWidth;
   uint32_t biHeight;
   uint16_t biBitCount;
   uint32_t biCompression;
};

struct WINBMPINFOHEADER
{
   uint32_t biWidth;
   uint32_t biHeight;
   uint16_t biPlanes;
   uint16_t biBitCount;
   uint32_t biCompression;
   uint32_t biSizeImage;
   uint32_t biXPelsPerMeter;
   uint32_t biYPelsPerMeter;
   uint32_t biClrUsed;
   uint32_t biClrImportant;
};

struct bmp_struct
{
   jmp_buf setjmp_buf;
   img32::Palette palette;
   int flags;
   FILE* infile;
   void* data;
   bmp_error_fn error_fn;
   bmp_error_fn warning_fn;
   BMPFILEHEADER bfHeader;
   BMPINFOHEADER biHeader;
};

void bmp_error(bmp_struct_ptr bmp_ptr, bmp_const_charp error_msg)
{
   if(bmp_ptr->error_fn) {
      bmp_ptr->error_fn(bmp_ptr, error_msg);
      longjmp(bmp_ptr->setjmp_buf, 1);
   }
}

bmp_struct_ptr bmp_create_decoder(void* error_ptr, bmp_error_fn error_fn, bmp_error_fn warning_fn)
{
   bmp_struct_ptr bmp_ptr = new bmp_struct;
   bmp_ptr->error_fn = error_fn;
   bmp_ptr->warning_fn = warning_fn;
   bmp_ptr->data = error_ptr;
   bmp_ptr->flags = NULL;

   return bmp_ptr;
}

void bmp_init_io(bmp_struct_ptr bmp_ptr, FILE* file)
{
   bmp_ptr->infile = file;
}

void bmp_fileheader(bmp_struct_ptr bmp_ptr)
{
   FILE* file = bmp_ptr->infile;
   BMPFILEHEADER* bfHandle = &bmp_ptr->bfHeader;

   bfHandle->bfType = read16(file);
   bfHandle->bfSize = read32(file);
   bfHandle->bfReserved1 = read16(file);
   bfHandle->bfReserved2 = read16(file);
   bfHandle->bfOffBits = read32(file);

   if(bfHandle->bfType != 0x4D42)
      bmp_error(bmp_ptr, "El archivo no es un BITMAP");
}

void bmp_winfileheader(bmp_struct_ptr bmp_ptr)
{
   WINBMPINFOHEADER winInfoHeader;
   BMPINFOHEADER* biHeader = &bmp_ptr->biHeader;
   FILE* file = bmp_ptr->infile;

   winInfoHeader.biWidth = read32(file);
   winInfoHeader.biHeight = read32(file);
   winInfoHeader.biPlanes = read16(file);
   winInfoHeader.biBitCount = read16(file);
   winInfoHeader.biCompression = read32(file);
   winInfoHeader.biSizeImage = read32(file);
   winInfoHeader.biXPelsPerMeter = read32(file);
   winInfoHeader.biYPelsPerMeter = read32(file);
   winInfoHeader.biClrUsed = read32(file);
   winInfoHeader.biClrImportant = read32(file);

   biHeader->biWidth = winInfoHeader.biWidth;
   biHeader->biHeight = winInfoHeader.biHeight;
   biHeader->biBitCount = winInfoHeader.biBitCount;
   biHeader->biCompression = winInfoHeader.biCompression;
}

void read_bmicolors(bmp_struct_ptr bmp_ptr, img32::ImageData* imgData, int bytes)
{
   int i, j, r, g, b;
   FILE* file = bmp_ptr->infile;
   
   for(i = j = 0; i+3 <= bytes && j < BMP_8BITS_MAX_PALETTE_SIZE;) {
      b = getc(file);
      g = getc(file);
      r = getc(file);

      if((bmp_ptr->flags & BMP_FLAGS_PALETTE_TO_RGB) != 0)
         img32::AddColorPaletteByColorType(&bmp_ptr->palette, imgData->getInfo().colorType(), r, g, b);
      else {
         imgData->getPalette()->addColor(img32::rgba(r, g, b));
      }

      j++;
      i += 3;

      if((bmp_ptr->flags & BMP_FLAGS_WIN) != 0 && i < bytes) {
         fgetc(file);
         i++;
      }
   }

   for(; i < bytes; i++)
      fgetc(file);
}

void bmp_read_info(bmp_struct_ptr bmp_ptr)
{
   if(!bmp_ptr->infile)
      bmp_error(bmp_ptr, "FILE no ha sido definido");

   bmp_fileheader(bmp_ptr);

   FILE* file = bmp_ptr->infile;
   uint32_t szHeader = read32(file);

   if(szHeader == WININFOHEADERSIZE) {
      bmp_ptr->flags |= BMP_FLAGS_WIN;
      bmp_winfileheader(bmp_ptr);
   }else {
      bmp_error(bmp_ptr, "El encabezado DIB no esta soportado");
   }

   if(bmp_ptr->biHeader.biWidth < 1 ||
      ABS(bmp_ptr->biHeader.biHeight) == 0)
      bmp_error(bmp_ptr, "Tamaño del BMP invalido!");

   uint32_t sizeBmp = bmp_ptr->biHeader.biWidth * ABS(bmp_ptr->biHeader.biHeight);
   if(bmp_ptr->biHeader.biBitCount >= 8)
      sizeBmp *= bmp_ptr->biHeader.biBitCount / 8;
   else if(8/bmp_ptr->biHeader.biBitCount > 0)
      sizeBmp /= 8/bmp_ptr->biHeader.biBitCount;

   if(sizeBmp > MAXBMPSIZE)
      bmp_error(bmp_ptr,
         format_to_string("Tamaño del BMP es mayor que el permitido (%.2f MB > %.2f MB)",
            sizeBmp / 1024.0 / 1024.0,
            MAXBMPSIZE / 1024.0 / 1024.0).c_str());
}

void read_8bit_line(bmp_struct_ptr bmp_ptr, img32::ImageData* imgData, int line)
{
   unsigned char b[4];
   unsigned long n;
   int x, j, k;
   int pix;

   for (x = 0; x < imgData->getInfo().width(); x++) {
      j = x % 4;
      if (j == 0) {
         n = read32(bmp_ptr->infile);
         for (k=0; k<4; k++) {
            b[k] = (char)(n & 255);
            n = n >> 8;
         }
      }

      pix = b[j];
      if((bmp_ptr->flags & BMP_FLAGS_PALETTE_TO_RGB) != 0) {
         *imgData->writable_addr32(x, line) = bmp_ptr->palette.getColor(pix);
      }else {
         *imgData->writable_addr8(x, line) = pix;
      }
   }
}

void read_24bit_line(bmp_struct_ptr bmp_ptr, img32::ImageData* imgData, int line)
{
   int x, r, g, b;
   img32::address_t dst_addr;

   for(x = 0; x < bmp_ptr->biHeader.biWidth; x++) {
      dst_addr = imgData->writable_addr32(x, line);
      b = fgetc(bmp_ptr->infile);
      g = fgetc(bmp_ptr->infile);
      r = fgetc(bmp_ptr->infile);
      img32::SetPixelAddressByColorType(dst_addr, imgData->getInfo().colorType(), r, g, b);
   }

   x = (3*x) % 4;
   if (x > 0)
      while (x++ < 4)
         fgetc(bmp_ptr->infile);
}

void read_image(bmp_struct_ptr bmp_ptr, img32::ImageData* imgData)
{
   int height = bmp_ptr->biHeader.biHeight;
   int line = height < 0 ? 0 : height-1;
   int dir = height < 0 ? 1 : -1;
   height = ABS(height);

   for(int y = 0; y < height; y++, line += dir) {
      switch(bmp_ptr->biHeader.biBitCount) {
      case 8: read_8bit_line(bmp_ptr, imgData, line); break;
      case 24: read_24bit_line(bmp_ptr, imgData, line); break;
      }
   }
}

void bmp_read_image(bmp_struct_ptr bmp_ptr, img32::ImageData* imgData)
{
   if(bmp_ptr->biHeader.biCompression != BI_BITFIELDS && bmp_ptr->biHeader.biBitCount <= 8)
      read_bmicolors(bmp_ptr, imgData, bmp_ptr->bfHeader.bfOffBits - 54);

   switch(bmp_ptr->biHeader.biCompression) {
   case BI_RGB:
      read_image(bmp_ptr, imgData);
      break;
   default:
      bmp_error(bmp_ptr, "Unsupported BMP compression");
      break;
   }
}

void bmp_get_info(bmp_struct_ptr bmp_ptr, int* width, int* height, int* color_type, int* bitcount, int* compression)
{
   #define SET(var, value) if(var) *var = value

   SET(width, bmp_ptr->biHeader.biWidth);
   SET(height, bmp_ptr->biHeader.biHeight);
   SET(bitcount, bmp_ptr->biHeader.biBitCount);
   SET(compression, bmp_ptr->biHeader.biCompression);

   if(bmp_ptr->biHeader.biBitCount <= 8)
      SET(color_type, BMP_COLOR_TYPE_INDEXED);
   else
      SET(color_type, BMP_COLOR_TYPE_RGB);
}

void bmp_set_palette_to_rgb(bmp_struct_ptr bmp_ptr)
{
   bmp_ptr->flags |= BMP_FLAGS_PALETTE_TO_RGB;
}

void bmp_destroy_decoder(bmp_struct_ptr* bmp_ptr)
{
   (*bmp_ptr)->data = nullptr;

   if(*bmp_ptr != nullptr) {
      delete *bmp_ptr;
      *bmp_ptr = nullptr;
   }
}

jmp_buf* bmp_setjmp_buf(bmp_struct_ptr bmp_ptr)
{
   return &bmp_ptr->setjmp_buf;
}

void* bmp_get_error_ptr(bmp_struct_ptr bmp_ptr)
{
   return bmp_ptr->data;
}