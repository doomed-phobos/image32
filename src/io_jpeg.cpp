#include "io_jpeg.h"

#include <cstdlib>
#include <cstdio>

extern "C"
{
#include "jpeglib.h"
}

image_t JpegIO::decode(const char filename[])
{
   FILE* srcFile;
   image_t image = {0};
   jpeg_decompress_struct cinfo;
   jpeg_error_mgr jerr;
   JDIMENSION buffer_height;
   JSAMPARRAY buffer = NULL;

   if((srcFile = fopen(filename, "rb")) == nullptr) {
      printf("Error al abrir el archivo %s\n", filename);
      return image_t();
   }

   cinfo.err = jpeg_std_error(&jerr);

   jpeg_create_decompress(&cinfo);
   jpeg_stdio_src(&cinfo, srcFile);
   (void)jpeg_read_header(&cinfo, TRUE);
   (void)jpeg_start_decompress(&cinfo);

   image.width = cinfo.output_width;
   image.height = cinfo.output_height;

   buffer_height = cinfo.rec_outbuf_height;
   buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
   if(!buffer) {
      jpeg_destroy_decompress(&cinfo);
      return image_t();
   }

   for (int c = 0; c<(int)buffer_height; c++) {
      buffer[c] = (JSAMPROW)malloc(sizeof(JSAMPLE) * cinfo.output_width * cinfo.output_components);
      if (!buffer[c]) {
      for (c--; c>=0; c--)
         free(buffer[c]);
         free(buffer);
         jpeg_destroy_decompress(&cinfo);
      return image_t();
      }
  }

   std::size_t for_rows = sizeof(address_t) * cinfo.output_height;
   std::size_t rowstride_bytes = image.width * 4;
   std::size_t required_size = for_rows + rowstride_bytes*image.height;
   image.pixels = new pixel_t[required_size];

   while(cinfo.output_scanline < cinfo.output_height) {
      JDIMENSION num_scanlines = jpeg_read_scanlines(&cinfo, buffer, buffer_height);
      address_t dst;
      address_t src;
//TODO: OBTENER LOS PIXELES
      for(int y = 0; y < num_scanlines; y++) {
         src = ((address_t*)buffer)[y];
         dst = (address_t)image.pixels[cinfo.output_scanline-1+y];
         for(int x = 0; x < image.width; x++) {
            int r = *(src++);
            int g = *(src++);
            int b = *(src++);
            //dst = (address_t)image.pixels[0];
            *(dst++) = 0;
            *(dst++) = 0;
            *(dst++) = 0;
            puts("A");
            //*(dst++) = 255;
         }
      }
   }

   return image;
}