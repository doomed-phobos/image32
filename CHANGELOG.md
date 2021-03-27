# Changelog
All changes in this project will be recorded in this file

### [0.1.0] - 2020-12-15
#### Added
-  Support for **JPG** images
#### Security
-  ~~In some large images the *libjpeg* release may cause failures (failed to create temporary file). It is solved if you run your program in administrator mode (pero es muy soso)~~

### [0.2.0] - 2020-12-20
#### Added
-  Support for **PNG** images
-  Now you can't deliberately resize images
#### Changed
-  Changed **struct image_t** to **class Image**

### [0.3.0] - 2020-12-21
#### Added
-  Support for **BMP** images

### [0.3.1] - 2020-12-21
#### Added
-  libpng: You can read images with different types of colors (INDEXED, RGBA, RGB)
#### Changed
-  ~~The detection by extension was changed to detection by content~~

### [0.3.2] - 2020-12-22
#### Added
-  Pixel format (RGB, RGBA, BGRA) is now allowed
### [0.4.0] - 2021-01-10
#### Added
-  Added decoding of different types of PNG
-  Pixels stored within a vector
-  Pixels R, G, B, A have been joined and set within a uint32_t
#### Changed
-  The function ```image_from_filename``` was changed to ```Image :: loadFromFilename```
#### Removed
-  Removed possible support for .gif and webp (will it be added in other versions?)
### [0.6.0] . 2021 -03-27
#### Added
-  Se agregó soporte para imagenes BMP de 8 bits
-  Se agregó mas soporte para errores (UnknownDecoder, Result, FileHandle)
-  Se creo la clase **FileHandle**
-  Se agregó **ASCII a WCHAR** sin importar en que plataforma estés
#### Changed
-  Se cambió ImageIO por ImageCodec
-  Se separó el "esqueleto de la imagen" (pixeles, tamaño, bits) de Image
-  Se separó ImageInfo, creando ColorInfo
-  Se eliminó ImageBuffer (no era necesario)
-  Se eliminó Image (quizas sea agrego despues como una clase que pueda manipular la imagen)