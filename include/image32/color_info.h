#ifndef _COLOR_INFO_H
#define _COLOR_INFO_H

namespace img32
{
   enum ColorType
   {
      Unknown_ColorType,
      RGBA_8888_ColorType,
      RGB_888x_ColorType,
      BGRA_8888_ColorType,
      ARGB_8888_ColorType,
      Unchanged_ColorType, // Hace que el color sea igual a la imagen
      Indexed_ColorType // Sirve solo como un rasgo, NO usar cuando la imagen no tiene el tipo de color INDEXED (si lo usas, solo se ignorará)
   };

   class ColorInfo
   {
   public:
      ColorInfo() = default;

      int bytesPerPixel() const;
      ColorType colorType() const;

      static ColorInfo MakeUnknown();
      static ColorInfo MakeUnchanged();
      static ColorInfo Make(ColorType ct);
   private:
      ColorInfo(ColorType ct);
      
      ColorType m_colorType;
   };
} // namespace img32


#endif