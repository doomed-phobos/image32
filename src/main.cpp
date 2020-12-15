#include "image.h"

#include <stdexcept>
#include <windows.h>

//TODO: El formato de Windows es BGRA

HWND g_window;
constexpr const char g_lpClassName[] = "Window";

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
int main()
{
   HINSTANCE hInstance = GetModuleHandle(nullptr);
   WNDCLASSEX wcex = {0};
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.hInstance = hInstance;
   wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
   wcex.lpszClassName = g_lpClassName;
   wcex.lpfnWndProc = WndProc;
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   if(!RegisterClassEx(&wcex)) throw std::runtime_error("Error al registar una clase!");
   
   g_window = CreateWindowEx(
      WS_EX_APPWINDOW,
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
   if(!g_window) throw std::runtime_error("Error al crear una ventana!");

   ShowWindow(g_window, SW_SHOW);
   UpdateWindow(g_window);

   MSG msg;
   while(GetMessage(&msg, nullptr, 0, 0) > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
// Add a reader image
   switch(msg) {
   case WM_CREATE: {
   }
   break;
   case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDC = BeginPaint(hWnd, &ps);
      static image_t image = {0};
      static bool init = false;

      if(!init) {
         init = true;
         if(!image_from_filename(&image, __argv[1])) {
            puts("Error al abrir la imagen!");
         }
      }

      BITMAPINFO bi = {0};
      bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bi.bmiHeader.biPlanes = 1;
      bi.bmiHeader.biWidth = image.width;
      bi.bmiHeader.biHeight = -image.height;
      bi.bmiHeader.biBitCount = 32;
      bi.bmiHeader.biCompression = BI_RGB;

      StretchDIBits(hDC,
      0, 0, image.width, image.height,
      0, 0, image.width, image.height, reinterpret_cast<void*>(image.pixels), &bi, DIB_RGB_COLORS, SRCCOPY);

      EndPaint(hWnd, &ps);
   }
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
   break;
   }

   return DefWindowProc(hWnd, msg, wParam, lParam);
}