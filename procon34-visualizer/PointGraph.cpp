# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"
# include "PointGraph.hpp"


PointGraph::PointGraph(void){}
PointGraph::PointGraph(const Array<int>& ary) {
	this->points = ary;
}

void PointGraph::draw(const Point &point, const Size &size, const int &limit) const {
	Rect(point, size).draw(color_background);
	Array<Point> coordinates;
	int max_point = *max_element(points.begin(), points.begin() + limit);
	for (int idx = 0; idx < points.size(); idx++) {
		const double x = point.x + size.x * ((double)idx / (double)limit);
		const double y = point.y + point.y - ((double)points[idx] / (double)max_point);
	}
}

void PointGraph::set_background(const Color &color){
	this->color_background = color;
}
