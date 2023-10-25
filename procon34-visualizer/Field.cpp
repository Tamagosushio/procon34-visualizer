# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Base.hpp"
# include "Field.hpp"



// フィールドの座標を引数に、セルの中心の画面上の座標を返す
constexpr Point get_cell_center(const Point &p){
	return Point(p.x * CELL_SIZE + BLANK_LEFT + CELL_SIZE / 2, p.y * CELL_SIZE + BLANK_TOP + CELL_SIZE / 2);
}
constexpr Rect get_grid_rect(const Point &p){
	return Rect{ p.x * CELL_SIZE + BLANK_LEFT, p.y * CELL_SIZE + BLANK_TOP, CELL_SIZE };
}
constexpr Rect get_grid_rect(const int y, const int x){
	return Rect{ x * CELL_SIZE + BLANK_LEFT, y * CELL_SIZE + BLANK_TOP, CELL_SIZE };
}
constexpr Circle get_grid_circle(const Point &p){
	return Circle(Arg::center(get_cell_center(p)), CELL_SIZE * 0.3);
}

// グリッド上の座標がグリッドに収まっているか
bool is_in_field(const int y, const int x) {
	return 0 <= y and y < HEIGHT and 0 <= x and x < WIDTH;
}
bool is_in_field(const Point &p) {
	return is_in_field(p.y, p.x);
}


// セルの情報を取得
CELL Field::get_cell(const int y, const int x) const {
	return this->grid[y][x];
}
CELL Field::get_cell(const Point p) const {
	return this->get_cell(p.y, p.x);
}
// セルの情報を変更
void Field::set_bit(const int y, const int x, const CELL new_bit) {
	this->grid[y][x] |= new_bit;
}
void Field::set_bit(const Point p, const CELL new_bit) {
	this->set_bit(p.y, p.x, new_bit);
}
// セルの情報を削除
void Field::delete_bit(const int y, const int x, const CELL delete_bit) {
	this->grid[y][x] &= ~delete_bit;
}
void Field::delete_bit(const Point p, const CELL delete_bit) {
	this->delete_bit(p.y, p.x, delete_bit);
}


void Field::display_grid(void) const {
	for (int i = 0; i < HEIGHT * WIDTH; i++) {
		get_grid_rect(Point(i % WIDTH, i / WIDTH)).drawFrame(1, 1, Palette::Black);
	}
}

void Field::display_actors(void) const {
	for (int i = 0; i < (HEIGHT * WIDTH); i++) {
		const int y = i / WIDTH;
		const int x = i % WIDTH;
		const Point p(x, y);
		const CELL target_cell = grid[y][x];

		if (target_cell & CELL::POND) {
			get_grid_rect(p).draw(Palette::Black);
		}
		if (target_cell & CELL::CASTLE) {
			Shape2D::Star(CELL_SIZE * 0.6, get_cell_center(p)).draw(Palette::Black);
		}
		if (target_cell & CELL::AREA_RED and target_cell & CELL::AREA_BLUE) {
			get_grid_rect(p).draw(ColorF(1.0, 0.0, 1.0, 0.5));
		}
		else 	if (target_cell & CELL::AREA_RED) {
			get_grid_rect(p).draw(ColorF(1.0, 0.0, 0.0, 0.25));
		}
		else 	if (target_cell & CELL::AREA_BLUE) {
			get_grid_rect(p).draw(ColorF(0.0, 0.0, 1.0, 0.25));
		}
		if (target_cell & CELL::WALL_RED) {
			Rect(Arg::center(get_cell_center(p)), (int)(CELL_SIZE * 0.7)).draw(Palette::Red);
		}
		if (target_cell & CELL::WALL_BLUE) {
			Rect(Arg::center(get_cell_center(p)), (int)(CELL_SIZE * 0.7)).draw(Palette::Blue);
		}
		if (target_cell & CELL::CRAFTSMAN_RED) {
			Circle(Arg::center(get_cell_center(p)), CELL_SIZE * 0.3).draw(ColorF(1.0, 0.5, 0.5));
			Circle(Arg::center(get_cell_center(p)), CELL_SIZE * 0.3).drawFrame(1, 1, Palette::White);
		}
		if (target_cell & CELL::CRAFTSMAN_BLUE) {
			Circle(Arg::center(get_cell_center(p)), CELL_SIZE * 0.3).draw(ColorF(0.5, 0.5, 1.0));
			Circle(Arg::center(get_cell_center(p)), CELL_SIZE * 0.3).drawFrame(1, 1, Palette::White);
		}
	}
}


