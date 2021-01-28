#include "image32/image32.h"

#include <iostream>
#include <stdexcept>
#include <windows.h>

#pragma comment(lib, "Msimg32.lib")

//TODO: El formato de Windows es BGRA

HWND g_window;
constexpr const char g_lpClassName[] = "Window";
img32::Image image(img32::ImageInfo::Make(9, 6, img32::BGRA_8888));

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class IOErrorDelegate : public img32::IOErrorDelegate
{
public:
   virtual void OnError(img32::const_charp msg) override {
      MessageBox(nullptr, msg, "Error decode", MB_OK);
   }
};

IOErrorDelegate errDelegate;
//FIXME: QUITAR LOS TYPEDEFs DE COLORES
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
int main()
{
   image.loadFromFilename("C:\\Users\\Usuario\\Desktop\\423beaf5a703c357b4d724a0cb692686-dibujos-animados-de-fuego-by-vexels.png", img32::BGRA_8888);
   HINSTANCE hInstance = GetModuleHandle(nullptr);
   WNDCLASSEX wcex = {0};
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.hInstance = hInstance;
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszClassName = g_lpClassName;
   wcex.lpfnWndProc = WndProc;
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   if(!RegisterClassEx(&wcex)) throw std::runtime_error("Error al registar una clase!");
   
   g_window = CreateWindowEx(
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
   case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDC = BeginPaint(hWnd, &ps);
      
      BITMAPINFO bi = {0};
      bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bi.bmiHeader.biPlanes = 1;
      bi.bmiHeader.biWidth = image.width();
      bi.bmiHeader.biHeight = -image.height();
      bi.bmiHeader.biBitCount = 32;
      bi.bmiHeader.biCompression = BI_RGB;
      
      HDC hOldDC = CreateCompatibleDC(hDC);
      void* pixels = image.getPixels();
      HBITMAP hBitmap = CreateDIBSection(hOldDC, &bi, DIB_RGB_COLORS, &pixels, NULL, 0x00);
      SelectObject(hOldDC, hBitmap);

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
               img32::ImageIO io(&str[0]);
               io.setErrorDelegate(&errDelegate);
               io.decode(&image, img32::BGRA_8888);
               //image.loadFromFilename(&str[0], img32::BGRA_8888);
               printf("Width: %d\n"
                      "Height: %d\n", image.width(), image.height());
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