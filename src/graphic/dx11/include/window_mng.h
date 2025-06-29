/**
 * @file window_mng.h
 * @brief ウィンドウ管理をまとめたかっただけのヘッダファイル
 * @author nekogakure
 */

#ifndef WINDOW_MNG_H
#define WINDOW_MNG_H

#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif