# pragma once
# include <Siv3D.hpp>
# include "Base.hpp"

constexpr CELL CELL::NONE = 0;
constexpr CELL CELL::POND = 1 << 0;
constexpr CELL CELL::WALL_RED = 1 << 1;
constexpr CELL CELL::WALL_BLUE = 1 << 2;
constexpr CELL CELL::AREA_RED = 1 << 3;
constexpr CELL CELL::AREA_BLUE = 1 << 4;
constexpr CELL CELL::CRAFTSMAN_RED = 1 << 5;
constexpr CELL CELL::CRAFTSMAN_BLUE = 1 << 6;
constexpr CELL CELL::CASTLE = 1 << 7;
constexpr CELL CELL::CRAFTSMAN = CELL::CRAFTSMAN_BLUE | CELL::CRAFTSMAN_RED;
constexpr CELL CELL::WALL = CELL::WALL_BLUE | CELL::WALL_RED;
constexpr CELL CELL::AREA = CELL::AREA_BLUE | CELL::AREA_RED;

constexpr TEAM TEAM::RED = 0;
constexpr TEAM TEAM::BLUE = 1;


CELL switch_cell(const CELL type, const TEAM team) {
	if (type == CELL::WALL) {
		return (team == TEAM::RED) ? CELL::WALL_RED : CELL::WALL_BLUE;
	}
	else if (type == CELL::AREA) {
		return (team == TEAM::RED) ? CELL::AREA_RED : CELL::AREA_BLUE;
	}
	else if (type == CELL::CRAFTSMAN) {
		return (team == TEAM::RED) ? CELL::CRAFTSMAN_RED : CELL::CRAFTSMAN_BLUE;
	}
	else {
		return CELL::NONE;
	}
}

Rect draw_button_rect(Rect rect, HSV button_color, const HSV& shadow_color){
	Vec2 shadow{ 0, rect.h / 7.5 };
	if (rect.mouseOver()) {
		button_color = button_color.setS(button_color.s * 4 / 5);
	}
	if (rect.leftPressed()) {
		rect = rect.stretched(-shadow.y, 0, shadow.y, 0);
		rect.rounded(10).draw(button_color);
	}
	else {
		rect.rounded(10).drawShadow(shadow, 5, 0, shadow_color).draw(button_color);
	}
	return rect;
}
void draw_button_label(const String& label, const Rect& rect, const Font& font, const HSV& color, bool is_center){
	int left = 0, right = 1000;
	while (Abs(right - left) > 1) {
		int mid = (left + right) / 2;
		if (font(label).draw(mid, rect.stretched(-rect.h / 20), HSV{ 0,0 })) {
			left = mid;
		}
		else {
			right = mid;
		}
	}
	if (is_center) {
		font(label).drawAt(left, rect.center(), color);
	}else{
		font(label).draw(left, rect.stretched(-rect.h / 20), color);
	}
}
