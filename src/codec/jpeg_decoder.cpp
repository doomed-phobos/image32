#include "jpeg_decoder.h"

#include "../image_priv.h"
#include "../string.h"

namespace img32
{
   JpegDecoder::JpegDecoder(const ColorType& colorType, CodecErrorCallback callback) :
      ImageDecoder(colorType, callback)
   {}

   JpegDecoder::~JpegDecoder()
   {
      jpeg_destroy_decompress(&m_dinfo);
   }

   Result JpegDecoder::init(FILE* file)
   {
      m_dinfo.err = jpeg_std_error(&m_err.jerr);
      m_err.jerr.error_exit = jpeg_error_exit;

      if(setjmp(m_err.setjmp_buf)) {
         jpeg_destroy_decompress(&m_dinfo);
         return Result::Failed;
      }

      jpeg_create_decompress(&m_dinfo);
      jpeg_stdio_src(&m_dinfo, file);

      return Result::Success;
   }
// TODO: Crear la clase ImageData que contenga un ImageInfo y buffer de pixeles
// TODO: Crear los decodificadores

   Result JpegDecoder::decode(ImageData* imgData)
   {
      (void)jpeg_read_header(&m_dinfo, true);

      if(m_dinfo.jpeg_color_space == JCS_GRAYSCALE)
         m_dinfo.out_color_space = JCS_GRAYSCALE;
      else
         m_dinfo.out_color_space = JCS_RGB;

      (void)jpeg_start_decompress(&m_dinfo);

      int width = m_dinfo.output_width;
      int height = m_dinfo.output_height;

      if(m_colorType == Unchanged_ColorType)
         m_colorType = RGB_888x_ColorType;

      imgData->reset(ImageInfo::Make(width, height, ColorInfo::Make(m_colorType)));

      JDIMENSION buffer_height = m_dinfo.rec_outbuf_height;
      JSAMPARRAY buffer = new JSAMPROW[buffer_height];
      if(!buffer) {
         jpeg_destroy_decompress(&m_dinfo);
         return Result::ErrorAllocationMemory;
      }

      for(int c = 0; c < (int)buffer_height; c++) {
         buffer[c] = new JSAMPLE[m_dinfo.output_width*m_dinfo.output_components];
         if(!buffer[c]) {
            for(c--; c >= 0; c--)
               delete[] buffer[c];
            delete[] buffer;
            jpeg_destroy_decompress(&m_dinfo);
            return Result::ErrorAllocationMemory;
         }
      }

      while(m_dinfo.output_scanline < m_dinfo.output_height) {
         JDIMENSION num_scanlines = jpeg_read_scanlines(&m_dinfo, buffer, buffer_height);
         uint8_t* src_address;
         uint32_t* dst_address;
         int r, g, b;

         if(m_dinfo.out_color_space == JCS_RGB) {
            for(int y = 0; y < (int)num_scanlines; y++) {
               src_address = ((uint8_t**)buffer)[y];
               for(int x = 0; x < imgData->getInfo().width(); x++) {
                  dst_address = imgData->writable_addr32(x, m_dinfo.output_scanline-1+y);
                  r = *(src_address++);
                  g = *(src_address++);
                  b = *(src_address++);
                  SetPixelAddressByColorType(dst_address, m_colorType, r, g, b);
               }
            }
         }else {
            for(int y = 0; y < (int)num_scanlines; y++) {
               src_address = ((uint8_t**)buffer)[y];
               for(int x = 0; x < imgData->getInfo().width(); x++) {
                  dst_address = imgData->writable_addr32(x, m_dinfo.output_scanline-1+y);
                  int grayscale = *(src_address++);
                  r = grayscale;
                  g = grayscale;
                  b = grayscale;
                  SetPixelAddressByColorType(dst_address, m_colorType, r, g, b);
               }
            }
         }
      }

      for(int c = 0; c < (int)buffer_height; c++)
         delete[] buffer[c];
      delete[] buffer;

      jpeg_finish_decompress(&m_dinfo);
      jpeg_destroy_decompress(&m_dinfo);
      return Result::Success;
   }

   void JpegDecoder::jpeg_error_exit(j_common_ptr cinfo)
   {
      error_mgr* err = (error_mgr*)cinfo->err;
      char buf[JMSG_LENGTH_MAX];

      err->jerr.format_message(cinfo, buf);
      err->decoder->onError(format_to_string("jpeglib: %s", buf).c_str());
      longjmp(err->setjmp_buf, 1);
   }
} // namespace img32