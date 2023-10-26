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

