#include "image32/image32.h"

#include <iostream>
#include <stdexcept>
#include <windows.h>

constexpr const char g_lpClassName[] = "Window";
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
img32::ImageData g_image;

int main()
{
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

      if(g_image.isValid()) {
         BITMAPINFO bi = {0};
         bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
         bi.bmiHeader.biPlanes = 1;
         bi.bmiHeader.biWidth = g_image.getInfo().width();
         bi.bmiHeader.biHeight = -g_image.getInfo().height();
         bi.bmiHeader.biBitCount = 32;
         bi.bmiHeader.biCompression = BI_RGB;

         StretchDIBits(hDC, 0, 0, g_image.getInfo().width(), g_image.getInfo().height(),
            0, 0, g_image.getInfo().width(), g_image.getInfo().height(),
            g_image.getPixels(), &bi, DIB_RGB_COLORS, SRCCOPY);
      }
      
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

               img32::ImageCodec codec(img32::Unchanged_ColorType);
               codec.setErrorCallback([](const char* msg){puts(msg);});
               codec.decode(&g_image, &str[0]);

               printf("Palette size: %d\n", g_image.getPalette()->size());
               uint32_t c = g_image.getPalette()->getColor(15);
               printf("15: %d %d %d\n", img32::getR(c), img32::getG(c), img32::getB(c));

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