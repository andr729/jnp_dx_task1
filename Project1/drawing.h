#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d2d1_3.h>
#include <vector>

struct Point2D {
	FLOAT cord[2];
	D2D1_POINT_2F toD2Point() const;
};

struct Point3D {
	FLOAT cord[3];

	// Makes 2d point from 3d using simple x,y,z -> x,y projection 
	Point2D to2d() const;
};

class WavyPointGridHandler {
	size_t size_x, size_z;
	FLOAT grid_width;
	std::vector<Point3D> points;
public:
	WavyPointGridHandler(size_t size_x, size_t size_z, FLOAT grid_width);
	void drawOn(ID2D1HwndRenderTarget* target, ID2D1SolidColorBrush* brush) const;
	void reset();
	void rotate(FLOAT alpha);
	void offset(FLOAT dx, FLOAT dy);

	template<auto Func>
	void applyWaveFunction() {
		for (size_t x = 0; x < size_x; x++) {
			for (size_t z = 0; z < size_z; z++) {
				auto cx = points[x * size_z + z].cord[0];
				auto cz = points[x * size_z + z].cord[2];
				// y = f(x, z)
				points[x * size_z + z].cord[1] = Func(cx, cz);
			}
		}
	}
};
