#include "io_priv.h"

#include "file.h"
#include "string.h"

#include <cstring>
#include <csetjmp>

#define BMP_FIELD 0x4D42
#define BMP_ERROR(bmp, msg) bmp->error_fn(bmp, msg)

namespace img32::priv
{
   typedef struct bmp_decoder* bmp_decoder_ptr;
   typedef void(*bmp_error_ptr)(bmp_decoder_ptr, const char*);
   typedef void* bmp_voidp;

   typedef struct bmp_header
   {
      size_t size;
      uint32_t width;
      uint32_t height;
      uint16_t bitCount;
      uint16_t planes;
      uint16_t compression;
   }*bmp_header_ptr;

   typedef struct bmp_decoder
   {
      FILE* infile;
      bmp_error_ptr error_fn;
      int array_offset;
      bmp_header header;
      jmp_buf _jmp_buf;
      bmp_voidp data; //Reserved. NOT USED
   }*bmp_decoder_ptr;

   void bmp_start_stdio(bmp_decoder_ptr bmp, FILE* file)
   {
      // Estableciendo el cursor al inicio
      fseek(file, 0, SEEK_SET);

      if(little_endian::read16(file) != BMP_FIELD) BMP_ERROR(bmp, "File not is bitmap");
      size_t file_size = little_endian::read16(file);

      bmp->infile = file;
   }

   void read_header_bmp(bmp_decoder_ptr bmp)
   {
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

   void read_24bit_bmp(Image* dstImg, int scanline, bmp_decoder_ptr bmp)
   {
      int i;
      address_t dst_address;
      FILE* file = bmp->infile;
      for(i = 0; i < dstImg->width(); i++) {
         color_t b = read8(file);
         color_t g = read8(file);
         color_t r = read8(file);
         dst_address = dstImg->writable_addr32(i, bmp->header.height-scanline-1);
         SetPixelsIntoAddress(dst_address, dstImg->colorType(), r, g, b, 255);
      }

      i = (3*i) % 4;
      if(i > 0)
         while(i++ < 4)
            read8(file);
   }

   void read_array_bmp(Image* dstImg, bmp_decoder_ptr bmp)
   {
      offset(bmp->infile, bmp->array_offset);

      for(uint32_t y = 0; y < bmp->header.height; y++) {
         switch(bmp->header.bitCount) {
         case 24:
            read_24bit_bmp(dstImg, y, bmp);
            break;
         default: BMP_ERROR(bmp, "Unsupported bitcount"); break;
         }
      }
   }

   void bmp_show_error(bmp_decoder_ptr bmp, const char* error_msg)
   {
      ImageIOPriv* err = (ImageIOPriv*)bmp->data;
      if(err) err->onError(format_to_string("bitmap: %s", error_msg).c_str());
      longjmp(bmp->_jmp_buf, 1);
   }

   void bmp_initialize(bmp_decoder_ptr bmp, bmp_voidp data, bmp_error_ptr error_fn)
   {
      bmp->data = data;
      bmp->error_fn = error_fn;
   }

   uint32_t bmp_get_width(bmp_decoder_ptr bmp) {return bmp->header.width;}
   uint32_t bmp_get_height(bmp_decoder_ptr bmp) {return bmp->header.height;}

   bool ImageIOPriv::bmp_decode(Image* dstImg)
   {
      FileHandle file = open_file(filename(), "rb");
      bmp_decoder bmp;

      bmp_initialize(&bmp, (bmp_voidp)this, bmp_show_error);
      if(setjmp(bmp._jmp_buf)) return false;

      bmp_start_stdio(&bmp, file.get());
      read_header_bmp(&bmp);
      
      dstImg->reset(ImageInfo::Make(bmp_get_width(&bmp), bmp_get_height(&bmp), colorType()));
      
      read_array_bmp(dstImg, &bmp);
      return true;
   }
} // namespace img32::priv
