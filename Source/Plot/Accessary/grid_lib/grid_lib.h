#ifndef GRID_LIB_H
#define GRID_LIB_H

#include "grid.h"

bool			InitGrid(HINSTANCE hInstance);
HWND			CreateGrid(HWND hWndParent, HINSTANCE hInst, DWORD dwStyle, int x, int y, int cx, int cy);
CGrid*			GetGrid(HWND hWnd);

#endif