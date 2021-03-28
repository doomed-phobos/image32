#include "palette.h"

#include <cassert>

namespace img32
{
   Palette::Palette(int ncolors)
   {
      assert(ncolors >= 0);
      resize(ncolors);
   }

   size_t Palette::size() const
   {
      return m_colors.size();
   }

   void Palette::resize(int ncolors)
   {
      assert(ncolors >= 0);
      m_colors.resize(ncolors, rgba(0, 0, 0, 255));
   }

   void Palette::addColor(uint32_t color)
   {
      resize(size()+1);
      setColor(size()-1, color);
   }

   void Palette::setColor(int i, uint32_t color)
   {
      assert(i >= 0 && i < size());

      m_colors[i] = color;
   }

   uint32_t Palette::getColor(int i) const
   {
      if(i >= 0 && i < size())
         return m_colors[i];
      
      return 0;
   }
} // namespace img32
