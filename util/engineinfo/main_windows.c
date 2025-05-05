/* Engine */
#include <gf_version.h>

/* External library */
#include <windows.h>

/* Standard */
#include <stdio.h>

#define GUI_BUTTON_OK 1000

HINSTANCE    hInst;
HWND	     button_ok;
HFONT	     monospace;
gf_version_t ver;
char	     vertxt[512];

void ShowBitmapSize(HWND hWnd, HDC hdc, const char* name, int x, int y, int w, int h) {
	HBITMAP hBitmap = LoadBitmap(hInst, name);
	BITMAP	bmp;
	HDC	hmdc;
	GetObject(hBitmap, sizeof(bmp), &bmp);
	hmdc = CreateCompatibleDC(hdc);
	SelectObject(hmdc, hBitmap);
	if(w == 0 && h == 0) {
		StretchBlt(hdc, x, y, bmp.bmWidth, bmp.bmHeight, hmdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	} else {
		StretchBlt(hdc, x, y, w, h, hmdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}
	DeleteDC(hmdc);
	DeleteObject(hBitmap);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch(msg) {
	case WM_COMMAND: {
		int trig = LOWORD(wp);
		int ev	 = HIWORD(wp);
		if(trig == GUI_BUTTON_OK && ev == BN_CLICKED) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;
	}
	case WM_CLOSE: {
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	case WM_CREATE: {
		RECT rc;
		int  sz;
		int  width;
		int  height;
		int  padding;

		GetClientRect(hWnd, &rc);
		width	= rc.right - rc.left;
		height	= rc.bottom - rc.top;
		sz	= height * 4 / 5;
		padding = height / 2 - sz / 2;

		monospace = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
		button_ok = CreateWindow("BUTTON", "&OK", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, width - padding - 40, height - padding - 20, 40, 20, hWnd, (HMENU)GUI_BUTTON_OK, hInst, NULL);
		break;
	}
	case WM_PAINT: {
		HDC	    hdc;
		PAINTSTRUCT ps;
		RECT	    rc;
		RECT	    tx;
		int	    sz;
		int	    width;
		int	    height;
		int	    padding;

		GetClientRect(hWnd, &rc);
		width	= rc.right - rc.left;
		height	= rc.bottom - rc.top;
		sz	= height * 4 / 5;
		padding = height / 2 - sz / 2;

		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, monospace);
		ShowBitmapSize(hWnd, hdc, "GOLDFISH_BMP", padding, padding, sz, sz);

		tx.left	  = padding * 2 + sz;
		tx.right  = rc.right;
		tx.top	  = padding;
		tx.bottom = rc.bottom - padding * 2;
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, vertxt, strlen(vertxt), &tx, DT_LEFT);
		EndPaint(hWnd, &ps);
		break;
	}
	default: {
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	}
	return 0;
}

BOOL InitApp(void) {
	WNDCLASSEX wc;
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.style	 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInst;
	wc.hIcon	 = LoadIcon(hInst, "GAME");
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_MENU);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = "nishbox";
	wc.hIconSm	 = LoadIcon(hInst, "GAME");
	return RegisterClassEx(&wc);
}

BOOL InitWindow(int nCmdShow) {
	HWND hWnd;
	RECT deskrc, rc;
	HWND hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, &deskrc);
	hWnd = CreateWindow("nishbox", "GoldFish Engine Info", (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX, 0, 0, 600, 125, NULL, 0, hInst, NULL);

	if(!hWnd) {
		return FALSE;
	}
	GetWindowRect(hWnd, &rc);
	SetWindowPos(hWnd, HWND_TOP, (deskrc.right - (rc.right - rc.left)) / 2, (deskrc.bottom - (rc.bottom - rc.top)) / 2, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow) {
	MSG  msg;
	BOOL bret;

	hInst = hCurInst;

	gf_version_get(&ver);
	vertxt[0] = 0;
	sprintf(vertxt + strlen(vertxt), "GoldFish Engine %s\n", ver.full);
	sprintf(vertxt + strlen(vertxt), "Build Date   : %s\n", ver.date);
	sprintf(vertxt + strlen(vertxt), "Thread Model : %s\n", ver.thread);
	sprintf(vertxt + strlen(vertxt), "Renderer     : %s on %s\n", ver.driver, ver.backend);

	if(!InitApp()) {
		return FALSE;
	}

	if(!InitWindow(nCmdShow)) {
		return FALSE;
	}

	while((bret = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if(bret == -1) {
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
