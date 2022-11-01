#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void tick();
void initDirect2D(HWND hwnd);
void recreateRenderTarget(HWND hwnd);
void destroyRenderTarget();
void destroyDirect2D();
void onPaint(HWND hwnd);

