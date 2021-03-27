#include "io_priv.h"

#include "file.h"
#include "string.h"

#include <cstring>
#include <csetjmp>
//TODO: Crear el io de imagenes BMP Grayscale
#define BMP_FIELD 0x4D42

#define BMP_INFO_HEADER_SIZE 40
#define BMP_HEADER_SIZE 54
#define BMP_DEFAULT_PLANES 1

#define BMP_ERROR(bmp, msg) bmp->error_fn(bmp, msg)

namespace img32::priv
{
   typedef struct bmp_struct* bmp_struct_ptr;
   typedef void(*bmp_error_ptr)(bmp_struct_ptr, const char*);
   typedef void* bmp_voidp;

   typedef struct bmp_header
   {
      size_t size;
      uint32_t width;
      uint32_t height;
      uint16_t bitCount;
      uint16_t planes;
      uint16_t compression;
      EncoderOptions::ColorType color_type;
      uint32_t filler;
   }*bmp_header_ptr;

   typedef struct bmp_struct
   {
      FILE* infile;
      bmp_error_ptr error_fn;
      int array_offset;
      bmp_header header;
      jmp_buf _jmp_buf;
      bmp_voidp data; //Reserved. NOT USED
   }*bmp_struct_ptr;

   uint32_t bmp_get_width(bmp_struct_ptr bmp) {return bmp->header.width;}
   uint32_t bmp_get_height(bmp_struct_ptr bmp) {return bmp->header.height;}
   EncoderOptions::ColorType bmp_get_color_type(bmp_struct_ptr bmp) {return bmp->header.color_type;}
   uint16_t bmp_get_bitcount(bmp_struct_ptr bmp) {return bmp->header.bitCount;}
   void bmp_set_info(bmp_struct_ptr bmp, int width, int height, EncoderOptions::ColorType color_type)
   {
      bmp->header.width = width;
      bmp->header.height = height;
      bmp->header.color_type = color_type;
   }

///////////////////////////////////////////////////////////////////////////////////////
/// Decoding
///////////////////////////////////////////////////////////////////////////////////////

   void bmp_stdio_src(bmp_struct_ptr bmp, FILE* file)
   {
      fseek(file, 0, SEEK_SET); // Estableciendo el cursor al inicio

      if(little_endian::read16(file) != BMP_FIELD) BMP_ERROR(bmp, "File not is bitmap");
      size_t file_size = little_endian::read16(file);

      bmp->infile = file;
   }
// TODO: Cambiar la forma de lectura
// En vez de que cuando esté leyendo las imagenes verifique el tipo de color,
// crear la funcion makeColorType en Image class que cambiará el buffer de pixeles
// TODO: Cambiar ImageIO a ImageCodec
// TODO: Crear un enum Result que tendrá difentes tipos de resultado (como Success, Invalid File, etc)
// TODO: Usar std::unique_ptr para usar memoria
   void read_header_bmp(bmp_struct_ptr bmp)
   {//TODO: Decodificar bmp 8 bits
      using namespace little_endian;
      offset(bmp->infile, 10);
      bmp->array_offset = read32(bmp->infile);
      bmp->header.size = read32(bmp->infile);
      bmp->header.width = read32(bmp->infile);
      bmp->header.height = read32(bmp->infile);
      bmp->header.planes = read16(bmp->infile);
      bmp->header.bitCount = read16(bmp->infile);
      bmp->header.compression = read32(bmp->infile);
   }

   void read_24bit_bmp(Image* dstImg, int scanline, bmp_struct_ptr bmp)
   {
      int i;
      FILE* file = bmp->infile;
      for(i = 0; i < dstImg->width(); i++) {
         channel_t b = read8(file);
         channel_t g = read8(file);
         channel_t r = read8(file);
         address_t dst_address = dstImg->writable_addr32(i, scanline);
         set_pixels_into_address(dst_address, dstImg->colorType(), r, g, b, 255);
      }

      i = (3*i) % 4;
      if(i > 0)
         while(i++ < 4)
            read8(file);
   }

   void read_8bit_bmp(Image* dstImg, int scanline, bmp_struct_ptr bmp)
   {
      unsigned char b[4];
      unsigned long n;
      int i, j, k;
      int pix;

      for (i=0; i<dstImg->width(); i++) {
         j = i % 4;
         if (j == 0) {
            n = little_endian::read32(bmp->infile);
            for (k=0; k<4; k++) {
               b[k] = (char)(n & 255);
               n = n >> 8;
            }
         }

         pix = b[j];
         dstImg->putPixel(i, scanline, pix);
         //set_pixels_into_address(dstImg->writable_addr32(i, scanline), dstImg->colorType(), getR(pix),)
      }
   }

   void read_array_bmp(Image* dstImg, bmp_struct_ptr bmp)
   {
      offset(bmp->infile, bmp->array_offset);
      int line = bmp_get_height(bmp)-1;

      for(uint32_t y = 0; y < bmp_get_height(bmp); y++, line -= 1) {
         switch(bmp_get_bitcount(bmp)) {
         case 8:
            read_8bit_bmp(dstImg, line, bmp);
            break;
         case 24:
            read_24bit_bmp(dstImg, line, bmp);
            break;
         default: BMP_ERROR(bmp, "Unsupported bitcount"); break;
         }
      }
   }
   
///////////////////////////////////////////////////////////////////////////////////////
/// Encoding
///////////////////////////////////////////////////////////////////////////////////////
   void bmp_stdio_dst(bmp_struct_ptr bmp, FILE* file)
   {
      bmp->infile = file;
   }

   void bmp_write_info(bmp_struct_ptr bmp)
   {
      using namespace little_endian;
      FILE* file = bmp->infile;
      int bpp = 0;
      int biSizeImage;
      int bfSize;

      switch(bmp_get_color_type(bmp) & ~EncoderOptions::Alpha_BitMask) {
      case EncoderOptions::RGB_ColorType:
         bpp = 24;
         break;
      case EncoderOptions::Grayscale_ColorType:
         bpp = 8;
      default:
         BMP_ERROR(bmp, "Unsupported color type!");
      }

      bmp->header.filler = int((32 - ((bmp_get_width(bmp)*bpp-1) & 31)-1) / 8);

      if(bpp > 8) {
         biSizeImage = (bmp_get_width(bmp)*3 + bmp->header.filler) * bmp_get_height(bmp);
         bfSize = BMP_HEADER_SIZE + biSizeImage;
      }

      write16(file, BMP_FIELD);              // Header field ("BM")
      write32(file, bfSize);                 // Size BMP file
      write16(file, NULL);                   // Reserved
      write16(file, NULL);                   // Reserved
      if(bpp > 8)
         write32(file, BMP_HEADER_SIZE);
      
      // BMP header
      write32(file, BMP_INFO_HEADER_SIZE);   // biSize
      write32(file, bmp_get_width(bmp));     // biWidth
      write32(file, bmp_get_height(bmp));    // biHeight
      write16(file, BMP_DEFAULT_PLANES);     // biPlanes
      write16(file, bpp);                    // biBitCount
      write32(file, NULL);                   // biCompression
      write32(file, biSizeImage);            // biSizeImage
      write32(file, 0xB12);                  // biXPelsPerMeter
      write32(file, 0xB12);                  // biYPelsPerMeter

      if(bpp > 8) {
         write32(file, NULL);                // biClrUsed
         write32(file, NULL);                // biClrImportant
      }
   }

   void bmp_write_image(bmp_struct_ptr bmp, const Image& img, const EncoderOptions& opt)
   {
      FILE* file = bmp->infile;

      for(int y = bmp_get_height(bmp)-1; y >= 0; y--) {
         for(int x = 0; x < bmp_get_width(bmp); x++) {
            color_t c = img.getPixel(x, y);

            if(bmp_get_color_type(bmp) & EncoderOptions::Alpha_BitMask) {
               if(getA(c) < 255)
                  c = combine_to_solid_color(c, opt.bmp_background);
            }else
               if(getA(c) < 255)
                  c = opt.bmp_background;
               
            if((bmp_get_color_type(bmp) & ~EncoderOptions::Alpha_BitMask) == EncoderOptions::RGB_ColorType) {
               fputc(getB(c), file);
               fputc(getG(c), file);
               fputc(getR(c), file);
            }else
               fputc(c, file);
         }

         for(int i = 0; i < bmp->header.filler; i++)
            fputc(0, file);
      }
   }

   void bmp_show_error(bmp_struct_ptr bmp, const char* error_msg)
   {
      ImageIOPriv* err = (ImageIOPriv*)bmp->data;
      if(err) err->onError(format_to_string("bitmap: %s", error_msg).c_str());
      longjmp(bmp->_jmp_buf, 1);
   }

   void bmp_initialize(bmp_struct_ptr bmp, bmp_voidp data, bmp_error_ptr error_fn)
   {
      bmp->data = data;
      bmp->error_fn = error_fn;
   }

   bool ImageIOPriv::bmp_decode(Image* dstImg, ColorType ct)
   {
      FileHandle hFile = open_file(filename(), "rb");
      FILE* file = hFile.get();
      bmp_struct bmp;

      bmp_initialize(&bmp, (bmp_voidp)this, bmp_show_error);
      if(setjmp(bmp._jmp_buf)) return false;

      bmp_stdio_src(&bmp, file);
      read_header_bmp(&bmp);
      
      dstImg->reset(ImageInfo::Make(bmp_get_width(&bmp), bmp_get_height(&bmp), ct));
      
      read_array_bmp(dstImg, &bmp);
      return true;
   }

   bool ImageIOPriv::bmp_encode(const Image& srcImg, const EncoderOptions& options)
   {
      FileHandle hFile = open_file(filename(), "wb");
      FILE* file = hFile.get();
      bmp_struct bmp;

      bmp_initialize(&bmp, (bmp_voidp)this, bmp_show_error);
      if(setjmp(bmp._jmp_buf)) return false;

      bmp_stdio_dst(&bmp, file);
      
      bmp_set_info(&bmp, srcImg.width(), srcImg.height(), options.colortype);
      bmp_write_info(&bmp);
      bmp_write_image(&bmp, srcImg, options);

      return true;
   }
} // namespace img32::priv