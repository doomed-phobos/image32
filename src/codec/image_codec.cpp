#include "codec/image_codec.h"

#include "image_decoder.h"
#include "png_decoder.h"
#include "jpeg_decoder.h"
#include "bmp_decoder.h"
#include "unknown_decoder.h"

#include <memory>

namespace img32
{
   ImageCodec::ImageCodec(const ColorType& colorType) :
      m_colorType(colorType),
      m_callback(nullptr)
   {

   }

   void ImageCodec::setErrorCallback(CodecErrorCallback callback)
   {
      m_callback = callback;
   }

   Result ImageCodec::decode(ImageData* imgData, const char* filename)
   {
      std::unique_ptr<ImageDecoder> decoder(new UnknownDecoder(m_callback));
      FileHandle fhandle(filename, "rb");
      if(!fhandle.isValid()) return fhandle.getStatus();

      switch(fhandle.getFileFormatByContent()) {
      case FileFormat::Jpeg:
         decoder.reset(new JpegDecoder(m_colorType, m_callback));
         break;
      case FileFormat::Png:
         decoder.reset(new PngDecoder(m_colorType, m_callback));
         break;
      case FileFormat::Bmp:
         decoder.reset(new BmpDecoder(m_colorType, m_callback));
         break;
      }

      Result result = decoder->init(fhandle.getStdFile());
      if(result != Result::Success) return result;

      return decoder->decode(imgData);
   }
} // namespace img32
