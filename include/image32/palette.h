#ifndef _PALETTE_H
#define _PALETTE_H
#include "color.h"

#include <vector>

namespace img32
{
   class Palette
   {
   public:
      Palette() = default;
      Palette(int ncolors);

      size_t size() const;

      void resize(int ncolors);

      void addColor(uint32_t color);
      uint32_t getColor(int i) const;
      void setColor(int i, uint32_t color);
   private:
      std::vector<uint32_t> m_colors;
   };
} // namespace img32

#endif