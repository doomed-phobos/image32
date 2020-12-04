#include <stdexcept>
#include <windows.h>

HWND g_window;
constexpr const char g_lpClassName[] = "Window";

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
   WNDCLASSEX wcex = {0};
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.hInstance = hInstance;
   wcex.lpszClassName = g_lpClassName;
   wcex.lpfnWndProc = WndProc;
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
   // B G R
   static uint8_t pixels[] = {
      0, 0, 255
   };

   switch(msg) {
   case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hDC = BeginPaint(hWnd, &ps);

      BITMAPINFO bi = {0};
      bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bi.bmiHeader.biPlanes = 1;
      bi.bmiHeader.biWidth = 1;
      bi.bmiHeader.biHeight = -1;
      bi.bmiHeader.biBitCount = 32;
      bi.bmiHeader.biCompression = BI_RGB;

      StretchDIBits(hDC,
      0, 0, 100, 100,
      0, 0, 1, 1, reinterpret_cast<void*>(pixels), &bi, DIB_RGB_COLORS, SRCCOPY);

      EndPaint(hWnd, &ps);
   }
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
   break;
   }

   return DefWindowProc(hWnd, msg, wParam, lParam);
}