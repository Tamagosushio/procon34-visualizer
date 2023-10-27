# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"


class PointGraph {
public:
	PointGraph(void);
	PointGraph(const Array<int>& ary);
	void draw(const Point &point, const Size &size, const int &limit) const;
	void set_background(const Color &color);
private:
	Array<int> points;
	Color color_background = HSV(0, 0, 0, 0);
};
