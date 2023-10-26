# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Base.hpp"



constexpr Point get_cell_center(const Point &p);
constexpr Rect get_grid_rect(const Point &p);
constexpr Rect get_grid_rect(const int y, const int x);
constexpr Circle get_grid_circle(const Point &p);

bool is_in_field(const int y, const int x);
bool is_in_field(const Point &p);

class Field {
public:
private:
	int height;
	int width;
	// セルの情報を取得する
	CELL get_cell(const int y, const int x) const;
	CELL get_cell(const Point p) const;
	// セルの情報を追加する
	void set_bit(const int y, const int x, const CELL new_bit);
	void set_bit(const Point p, const CELL new_bit);
	// セルの情報を削除する
	void delete_bit(const int y, const int x, const CELL delete_bit);
	void delete_bit(const Point p, const CELL delete_bit);
	// 盤面のグリッドを表示させる
	void display_grid(void) const;
	// 盤面を表示させる
	void display_actors(void) const;
	// グリッド上の座標がグリッドに収まっているか
	bool is_in_field(const int y, const int x);
	bool is_in_field(const Point& p);
	// 盤面情報
	Array<Array<CELL>> grid;
};


