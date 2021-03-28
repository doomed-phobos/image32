#ifndef _LIBBMP_H
#define _LIBBMP_H
#include <csetjmp>
#include <cstdio>

#define bmp_setjmp(bmp_ptr) *bmp_setjmp_buf(bmp_ptr)
#define BI_RGB          0
#define BI_RLE8         1
#define BI_RLE4         2
#define BI_BITFIELDS    3

#define BMP_COLOR_TYPE_RGB       4
#define BMP_COLOR_TYPE_INDEXED   5

namespace img32 {class ImageData;}

typedef struct bmp_struct* bmp_struct_ptr;
typedef const char* bmp_const_charp;
typedef void(*bmp_error_fn)(bmp_struct_ptr bmp_ptr, bmp_const_charp error_msg);

jmp_buf* bmp_setjmp_buf(bmp_struct_ptr bmp_ptr);
void* bmp_get_error_ptr(bmp_struct_ptr bmp_ptr);

bmp_struct_ptr bmp_create_decoder(void* error_ptr, bmp_error_fn error_fn, bmp_error_fn warning_fn);
void bmp_init_io(bmp_struct_ptr bmp_ptr, FILE* file);
void bmp_read_info(bmp_struct_ptr bmp_ptr);
void bmp_read_image(bmp_struct_ptr bmp_ptr, img32::ImageData* imgData);

void bmp_get_info(bmp_struct_ptr bmp_ptr, int* width, int* height, int* color_type, int* bitcount, int* compression);
void bmp_set_palette_to_rgb(bmp_struct_ptr bmp_ptr);

void bmp_destroy_decoder(bmp_struct_ptr* bmp_ptr);

#endif