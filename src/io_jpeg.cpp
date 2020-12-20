#include "io_jpeg.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

extern "C"
{
#include "jpeglib.h"
}

namespace img32
{
   JpegIO::JpegIO(const char filename[])
   {
      m_file = fopen(filename, "rb");
   }

   bool JpegIO::decode(Image* dstImg)
   {
      if(!m_file) return false;

      jpeg_decompress_struct dinfo;
      jpeg_error_mgr jerr;

      dinfo.err = jpeg_std_error(&jerr);

      jpeg_create_decompress(&dinfo);
      jpeg_stdio_src(&dinfo, m_file);
      (void)jpeg_read_header(&dinfo, TRUE);
      (void)jpeg_start_decompress(&dinfo);

      int width = dinfo.output_width;
      int height = dinfo.output_height;
      ColorSpace cs = (dinfo.out_color_space == JCS_RGB ? ColorSpace::RGB : ColorSpace::GRAYSCALE);

      *dstImg = Image::Make(width, height, cs);

      JDIMENSION buffer_height = dinfo.rec_outbuf_height;
      JSAMPARRAY buffer = new JSAMPROW[buffer_height];
      if (!buffer) {
         jpeg_destroy_decompress(&dinfo);
         return false;
      }

      for (int c=0; c<(int)buffer_height; c++) {
         buffer[c] = new JSAMPLE[dinfo.output_width * dinfo.output_components];
         if (!buffer[c]) {
            for (c--; c>=0; c--)
               delete[] buffer[c];
            delete[] buffer;
            jpeg_destroy_decompress(&dinfo);
            return false;
         }
      }
        
      while(dinfo.output_scanline < dinfo.output_height) {
         JDIMENSION num_scanlines = jpeg_read_scanlines(&dinfo, buffer, buffer_height);

         if(dstImg->colorSpace() == ColorSpace::RGB) {
            uint8_t* src_address;
            uint8_t* dst_address;

               for (int y=0; y<(int)num_scanlines; y++) {
                  src_address = ((uint8_t**)buffer)[y];
                  dst_address = (uint8_t*)dstImg->getPixelAddress(0, dinfo.output_scanline-1+y);

                  for (int x=0; x<dstImg->width(); x++) {
                     int r = *(src_address++);
                     int g = *(src_address++);
                     int b = *(src_address++);
                     //TODO: BGRA
                     *(dst_address++) = b;
                     *(dst_address++) = g;
                     *(dst_address++) = r;
                     *(dst_address++) = 255;
                  }
               }
         }
      }
      
      for(int i = 0; i < (int)buffer_height; i++)
         delete[] buffer[i];
      delete[] buffer;

      jpeg_finish_decompress(&dinfo);
      jpeg_destroy_decompress(&dinfo);

      return true;
   }
} // namespace img32