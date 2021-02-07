#include "image32/image_buffer.h"

namespace img32
{
   ImageBuffer::ImageBuffer(size_t size) :
   m_buffer(size)
   {}

   size_t ImageBuffer::size() const
   {
      return m_buffer.size();
   }

   const uint8_t* ImageBuffer::buffer() const
   {
      return &m_buffer[0];
   }

   uint8_t* ImageBuffer::buffer()
   {
      return &m_buffer[0];
   }

   void ImageBuffer::resize(size_t size)
   {
      if(size > m_buffer.size())
         m_buffer.resize(size);
   }
} // namespace img32
