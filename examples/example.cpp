#include "image32/image32.h"

#include <iostream>
#include <stdexcept>
#include <windows.h>

//TODO: El formato de Windows es BGRA

constexpr const char g_lpClassName[] = "Window";
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
img32::Image g_image(img32::ImageInfo(3, 1, img32::RGBA_8888));


class IOErrorDelegate : public img32::IOErrorDelegate
{
public:
   virtual void OnError(img32::const_charp msg) override {
      MessageBox(nullptr, msg, "Error decode", MB_OK);
   }
};

int main()
{
   g_image.loadFromFilename("C:\\Users\\Usuario\\Desktop\\YlzsO.png", img32::ARGB_8888);
   img32::EncoderOptions eo;
   eo.colortype = img32::EncoderOptions::RGB_ColorType;
   eo.jpg_quality = 5;
   eo.jpg_background = img32::rgba(255, 0, 0);
   eo.fix_grayscale = false;
   g_image.saveToFilename("C:\\Users\\Usuario\\Desktop\\example.jpg", eo);

   HINSTANCE hInstance = GetModuleHandle(nullptr);
   WNDCLASSEX wcex = {0};
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.hInstance = hInstance;
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszClassName = g_lpClassName;
   wcex.lpfnWndProc = WndProc;
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   if(!RegisterClassEx(&wcex)) throw std::runtime_error("Error al registar una clase!");
   
   HWND window = CreateWindowEx(
      WS_EX_APPWINDOW | WS_EX_ACCEPTFILES,
      g_lpClassName,
      "Image Viewer",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      800, 400,
      nullptr,
      nullptr,
      hInstance,
      nullptr
   );
   if(!window) throw std::runtime_error("Error al crear una ventana!");

   ShowWindow(window, SW_SHOW);
   UpdateWindow(window);

   MSG msg;
   while(GetMessage(&msg, nullptr, 0, 0) > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

   switch(msg) {
   case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDC = BeginPaint(hWnd, &ps);
      
      BITMAPINFO bi = {0};
      bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bi.bmiHeader.biPlanes = 1;
      bi.bmiHeader.biWidth = g_image.width();
      bi.bmiHeader.biHeight = -g_image.height();
      bi.bmiHeader.biBitCount = 32;
      bi.bmiHeader.biCompression = BI_RGB;

      StretchDIBits(hDC, 0, 0, g_image.width(), g_image.height(),
         0, 0, g_image.width(), g_image.height(),
         g_image.getPixels(), &bi, DIB_RGB_COLORS, SRCCOPY);
      
      EndPaint(hWnd, &ps);
   }
      break;
   case WM_DROPFILES: {
      HDROP hdrop = (HDROP)(wParam);
      
      int count = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
      for (int index=0; index<count; ++index) {
            int length = DragQueryFile(hdrop, index, NULL, 0);
            if (length > 0) {
               std::vector<TCHAR> str(length+1);
               DragQueryFile(hdrop, index, &str[0], str.size());
               //img32::ImageIO io(&str[0]);
               //io.setErrorDelegate(&errDelegate);
               //io.decode(&image, img32::BGRA_8888);
               g_image.loadFromFilename(&str[0], img32::BGRA_8888);

               InvalidateRect(hWnd, nullptr, TRUE);
            }
      }

      DragFinish(hdrop);
   }
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
   break;
   }

   return DefWindowProc(hWnd, msg, wParam, lParam);
}