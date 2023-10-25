# pragma once
# include <Siv3D.hpp>
# include "Base.hpp"

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
