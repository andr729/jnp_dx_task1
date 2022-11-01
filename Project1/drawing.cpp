#include "drawing.h"

D2D1_POINT_2F Point2D::toD2Point() const {
	return D2D1::Point2F(cord[0], cord[1]);
}

Point2D Point3D::to2d() const {
	return { cord[0], cord[1] };
}

WavyPointGridHandler::WavyPointGridHandler(
	size_t size_x, size_t size_z, FLOAT grid_width) {

	this->size_x = size_x;
	this->size_z = size_z;
	this->grid_width = grid_width;
	points.resize(size_x * size_z);
	reset();
}

void WavyPointGridHandler::reset() {
	for (size_t x = 0; x < size_x; x++) {
		for (size_t z = 0; z < size_z; z++) {
			auto& p = points[x * size_z + z];
			p = { grid_width * x, 0, grid_width * z };
			p.cord[0] -= grid_width * size_x / 2;
			p.cord[2] -= grid_width * size_z / 2;
		}
	}
}

void WavyPointGridHandler::rotate(FLOAT aplha) {
	auto cos_a = cosf(aplha);
	auto sin_a = sinf(aplha);
	for (size_t x = 0; x < size_x; x++) {
		for (size_t z = 0; z < size_z; z++) {
			auto& p = points[x * size_z + z];
			auto cy = p.cord[1];
			auto cz = p.cord[2];
			p = { p.cord[0], cy * cos_a - cz * sin_a, cy * sin_a + cz * cos_a};
		}
	}
}

void WavyPointGridHandler::offset(FLOAT dx, FLOAT dy) {
	for (size_t x = 0; x < size_x; x++) {
		for (size_t z = 0; z < size_z; z++) {
			auto& p = points[x * size_z + z];
			p.cord[0] += dx;
			p.cord[1] += dy;
		}
	}
}

void WavyPointGridHandler::drawOn(
	ID2D1HwndRenderTarget* target,
	ID2D1SolidColorBrush* brush) const {

	for (size_t x = 0; x < size_x; x++) {
		for (size_t z = 0; z < size_z; z++) {
			auto point_2d_from = points[x * size_z + z].to2d();
			if (x < size_x - 1) {
				auto point_2d_to = points[x * size_z + z + size_z].to2d();
				target->DrawLine(
					point_2d_from.toD2Point(), 
					point_2d_to.toD2Point(),
					brush);
			}

			if (z < size_z - 1) {
				auto point_2d_to = points[x * size_z + z + 1].to2d();
				target->DrawLine(
					point_2d_from.toD2Point(),
					point_2d_to.toD2Point(),
					brush);
			}
		}
	}
}

