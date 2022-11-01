#include <d2d1_3.h>
#include "app.h"
#include "drawing.h"

// This line is needed on my local computer for some reason
// I suspose I have errors in linker configuration
#pragma comment(lib, "d2d1")

namespace {
	ID2D1Factory7* d2d_factory = nullptr;
	ID2D1HwndRenderTarget* d2d_render_target = nullptr;
	ID2D1SolidColorBrush* brush = nullptr;

	const D2D1_COLOR_F background_color =
		{ .r = 1.0f, .g = 0.4f, .b = 0.4f, .a = 1.0f };
	const D2D1_COLOR_F main_color =
		{ .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };

	WavyPointGridHandler points(51, 51, 10);
	UINT64 time = 0;

	FLOAT window_size_x;
	FLOAT window_size_y;
}

void tick() {
	time += 1;
}


void initDirect2D(HWND hwnd) {
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);
	if (d2d_factory == NULL) {
		exit(1);
	}
	recreateRenderTarget(hwnd);
}

void recreateRenderTarget(HWND hwnd) {
	RECT rc;
	GetClientRect(hwnd, &rc);

	window_size_x = rc.right - rc.left;
	window_size_y = rc.bottom - rc.top;

	d2d_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd,
			D2D1::SizeU(static_cast<UINT32>(rc.right) -
				static_cast<UINT32>(rc.left),
				static_cast<UINT32>(rc.bottom) -
				static_cast<UINT32>(rc.top))),
		&d2d_render_target);

	if (d2d_render_target == NULL) {
		exit(3);
	}

	if (brush) {
		brush->Release();
	}
	d2d_render_target->CreateSolidColorBrush(main_color, &brush);
}

void destroyRenderTarget() {
	if (d2d_render_target) {
		d2d_render_target->Release();
		d2d_render_target = nullptr;
	}
}

void destroyDirect2D() {
	if (d2d_render_target) d2d_render_target->Release();
	if (d2d_factory) d2d_factory->Release();
}

FLOAT wave(FLOAT x, FLOAT y) {
	x /= (51 / 2);
	y /= (51 / 2);
	return 50 * cosf(sqrt(10 * (x*x + y*y)) / 4);
}

void onPaint(HWND hwnd) {
	if (!d2d_render_target) recreateRenderTarget(hwnd);

	d2d_render_target->BeginDraw();
	d2d_render_target->Clear(background_color);

	points.reset();
	points.applyWaveFunction<wave>();
	points.rotate(time / 100.f);
	points.offset(window_size_x / 2, window_size_y / 2);
	points.drawOn(d2d_render_target, brush);

	if (d2d_render_target->EndDraw() == D2DERR_RECREATE_TARGET) {
		destroyRenderTarget();
		onPaint(hwnd);
	}
}