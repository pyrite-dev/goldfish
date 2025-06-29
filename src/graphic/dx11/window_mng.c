#include "include/window_mng.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CLOSE:

            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}