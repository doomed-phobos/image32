#include "io_jpeg.h"

#include "string.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

extern "C"
{
#include "jpeglib.h"
}

JpegIO::JpegIO(const char filename[])
{
   m_file = _wfopen(from_utf8(filename).c_str(), L"rb");
}

bool JpegIO::decode(image_t* dstImg)
{
   if(!m_file) return false;

   jpeg_decompress_struct dinfo;
   jpeg_error_mgr jerr;

   dinfo.err = jpeg_std_error(&jerr);

   jpeg_create_decompress(&dinfo);
   jpeg_stdio_src(&dinfo, m_file);
   (void)jpeg_read_header(&dinfo, TRUE);
   (void)jpeg_start_decompress(&dinfo);

   dstImg->width = dinfo.output_width;
   dstImg->height = dinfo.output_height;
   dstImg->colorSpace = (ColorSpace)dinfo.out_color_space;


   JDIMENSION buffer_height = dinfo.rec_outbuf_height;
   JSAMPARRAY buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
   if (!buffer) {
      jpeg_destroy_decompress(&dinfo);
      return false;
   }

   for (int c=0; c<(int)buffer_height; c++) {
      buffer[c] = (JSAMPROW)malloc(sizeof(JSAMPLE) *
                                       dinfo.output_width * dinfo.output_components);
      if (!buffer[c]) {
         for (c--; c>=0; c--)
            free(buffer[c]);
         free(buffer);
         jpeg_destroy_decompress(&dinfo);
         return false;
      }
   }
  
   std::size_t for_rows = sizeof(address_t) * dinfo.output_height;
   std::size_t rowstride_bytes = dstImg->width * 4;
   std::size_t required_size = for_rows + rowstride_bytes*dstImg->height;

   dstImg->pixels = new pixel_t[required_size];
   //memset(dstImg->pixels, 0, sizeof(pixel_t) * required_size);
   uint32_t xy = 0;
      
   while(dinfo.output_scanline < dinfo.output_height) {
      JDIMENSION num_scanlines = jpeg_read_scanlines(&dinfo, buffer, buffer_height);

      if(dstImg->colorSpace == ColorSpace::RGB) {
         uint8_t* src_address;
         int x, y;

            for (y=0; y<(int)num_scanlines; y++) {
               src_address = ((uint8_t**)buffer)[y];

               for (x=0; x<dstImg->width; x++) {
                  int r = *(src_address++);
                  int g = *(src_address++);
                  int b = *(src_address++);

                  dstImg->pixels[xy++] = b;
                  dstImg->pixels[xy++] = g;
                  dstImg->pixels[xy++] = r;
                  dstImg->pixels[xy++] = 255;
               }
            }
      }
   }

   return true;
}