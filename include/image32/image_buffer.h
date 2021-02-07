#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H
#include "config.h"

#include <vector>

namespace img32
{
   class IMG32_DLL ImageBuffer
   {
   public:
      typedef std::vector<uint8_t>::iterator iterator;

      ImageBuffer(size_t size = 1);

      size_t size() const;
      const uint8_t* buffer() const;
      uint8_t* buffer();

      void resize(size_t size);

      iterator begin() {return m_buffer.begin();}
      iterator end() {return m_buffer.end();}
   private:
      std::vector<uint8_t> m_buffer;
   };
} // namespace img32


#endif