# Changelog
All changes in this project will be recorded in this file

### [0.1.0] - 2020-12-15
#### Added
-  Support for **JPG** images
#### Security
-  In some large images the *libjpeg* release may cause failures (failed to create temporary file). It is solved if you run your program in administrator mode (pero es muy soso)

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
-  The detection by extension was changed to detection by content

### [0.3.2] - 2020-12-22
#### Added
-  Pixel format (RGB, RGBA, BGRA) is now allowed
//TODO: CAMBIAR A INGLES
### [0.4.0] - 2021-01-10
#### Added
-  Se añadió la decodificacion de diferentes tipos de PNG
#### Changed
-  Se cambio la funcion ```image_from_filename``` a ```Image::loadFromFilename```
#### Removed
-  Se elimino la posible compatibilidad para .gif y webp (¿se agregará en otras versiones?)