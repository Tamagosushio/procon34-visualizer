# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Base.hpp"
# include "FieldScene.hpp"



FieldScene::FieldScene(const InitData &init) : IScene(init) {
	// 前シーンから受け取った試合IDとフィールドタイプをキーにして情報を取り込む
	this->match_id = getData().get_match_id();
	this->field_type = getData().get_field_type();
	this->matchcsv = getData().get_loadmatches().get_match_csv(match_id);
	this->matchjson = getData().get_loadmatches().get_match_json(match_id);
	this->grid = getData().get_loadmatches().get_field_csv(field_type);
	height = grid.size(); width = grid.front().size();
	// 先攻チームと後攻チームの名前
	team_first = (match_id % 2 == 0) ? matchcsv.team1 : matchcsv.team2;
	team_second = (match_id % 2 == 0) ? matchcsv.team2 : matchcsv.team1;
	// ターン数に関する初期化
	this->turn_now = 0;
	this->turn_limit = matchjson.turns.size();
}


void FieldScene::update(void) {

}
void FieldScene::draw(void) const {

}


// フィールドの座標を引数に、セルの中心の画面上の座標を返す
constexpr Point FieldScene::get_cell_center(const Point &p)const{
	return Point(p.x * CELL_SIZE + BLANK_LEFT + CELL_SIZE / 2, p.y * CELL_SIZE + BLANK_TOP + CELL_SIZE / 2);
}
constexpr Rect FieldScene::get_grid_rect(const Point &p)const {
	return Rect{ p.x * CELL_SIZE + BLANK_LEFT, p.y * CELL_SIZE + BLANK_TOP, CELL_SIZE };
}
constexpr Rect FieldScene::get_grid_rect(const int y, const int x)const {
	return Rect{ x * CELL_SIZE + BLANK_LEFT, y * CELL_SIZE + BLANK_TOP, CELL_SIZE };
}
constexpr Circle FieldScene::get_grid_circle(const Point &p)const {
	return Circle(Arg::center(get_cell_center(p)), CELL_SIZE * 0.3);
}

// グリッド上の座標がグリッドに収まっているか
bool FieldScene::is_in_field(const int y, const int x)const {
	return 0 <= y and y < height and 0 <= x and x < width;
}
bool FieldScene::is_in_field(const Point& p)const {
	return is_in_field(p.y, p.x);
}


// セルの情報を取得
CELL FieldScene::get_cell(const int y, const int x) const {
	return this->grid[y][x];
}
CELL FieldScene::get_cell(const Point p) const {
	return this->get_cell(p.y, p.x);
}
// セルの情報を変更
void FieldScene::set_bit(const int y, const int x, const CELL new_bit) {
	this->grid[y][x] |= new_bit;
}
void FieldScene::set_bit(const Point p, const CELL new_bit) {
	this->set_bit(p.y, p.x, new_bit);
}
// セルの情報を削除
void FieldScene::delete_bit(const int y, const int x, const CELL delete_bit) {
	this->grid[y][x] &= ~delete_bit;
}
void FieldScene::delete_bit(const Point p, const CELL delete_bit) {
	this->delete_bit(p.y, p.x, delete_bit);
}


// フィールド更新
void FieldScene::update_walls(const Array<Array<int>>& walls) {
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			const int num = walls[h][w];
			delete_bit(h, w, CELL::WALL);
			if (num == 0);
			else if (num == 1) {
				set_bit(h, w, CELL::WALL_RED);
			}else if (num == 2) {
				set_bit(h, w, CELL::WALL_BLUE);
			}
		}
	}
}
void FieldScene::update_territories(const Array<Array<int>>& territories) {
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			const int num = territories[h][w];
			delete_bit(h, w, CELL::AREA);
			if (num == 0);
			else if (num == 1) {
				set_bit(h, w, CELL::AREA_RED);
			}else if (num == 2) {
				set_bit(h, w, CELL::AREA_BLUE);
			}else if (num == 3) {
				set_bit(h, w, CELL::AREA);
			}
		}
	}
}
void FieldScene::update_masons(const Array<Array<int>>& masons) {
	int craftsmen_num = 0;
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			const int num = masons[h][w];
			if (num > 0) craftsmen_num++;
		}
	}
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			const int num = masons[h][w];
			delete_bit(h, w, CELL::CRAFTSMAN);
			if (num > 0) {
				set_bit(h, w, CELL::CRAFTSMAN_RED);
			}else if (num < 0) {
				set_bit(h, w, CELL::CRAFTSMAN_BLUE);
			}
		}
	}
}
void FieldScene::update_turn(void) {
	// ボタンによる再生の有無
	if (SimpleGUI::Button((is_paused ? U"再生" : U"停止"), { Scene::Center().x, 50 })) {
		stopwatch.reset();
		is_paused ^= true;
	}
	// GUIによるターン遷移
	double turn_rate = (double)turn_now / (double)turn_limit;
	if (SimpleGUI::Slider(turn_rate, { 0,0 }, Scene::Size().x)) {
		turn_now = turn_rate * turn_limit;
		// GUIの操作中は時間経過のターン遷移を行わない
		stopwatch.restart();
	}
	if (not is_paused) {
		// 時間によるターン遷移
		if (not stopwatch.isStarted()) {
			stopwatch.start();
		}
		if (stopwatch.ms() > 1000) {
			turn_now = Min(turn_now + 1, turn_limit);
			stopwatch.restart();
		}
	}
}

void FieldScene::draw_gridlines(void) const {
	for (int i = 0; i < height * width; i++) {
		get_grid_rect(Point(i % width, i / width)).drawFrame(1, 1, Palette::Black);
	}
}
void FieldScene::draw_actors(void) const {
	for (int i = 0; i < (height * width); i++) {
		const int y = i / width;
		const int x = i % width;
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
void FieldScene::draw_details(void) const {

	//normal_font(U"赤ポイント:{}"_fmt(field.get_point(TEAM::RED))).draw(800, 50, ((now_turn == TEAM::RED) ? Palette::Red : Palette::Black));
	//small_font(U"城壁:{}  陣地:{}  城:{}"_fmt(building_red[0], building_red[1], building_red[2])).draw(850, 125, ((now_turn == TEAM::RED) ? Palette::Red : Palette::Black));
	//normal_font(U"青ポイント:{}"_fmt(field.get_point(TEAM::BLUE))).draw(800, 200, ((now_turn == TEAM::BLUE) ? Palette::Blue : Palette::Black));
	//small_font(U"城壁:{}  陣地:{}  城:{}"_fmt(building_blue[0], building_blue[1], building_blue[2])).draw(850, 275, ((now_turn == TEAM::BLUE) ? Palette::Blue : Palette::Black));
	//const int point_diff = field.get_point(TEAM::RED) - field.get_point(TEAM::BLUE);
	//normal_font(U"点差:{}"_fmt(point_diff)).draw(800, 350, (point_diff >= 0) ? ((point_diff == 0) ? Palette::Black : Palette::Red) : Palette::Blue);
	//normal_font(U"ターン数:{}/{}"_fmt(turn_num_now + 1, turn_num)).draw(800, 450, Palette::Black);
	//if (now_turn == TEAM::RED) {
	//	small_font(U"{} / {}"_fmt(this->time * time_limit_rate - stopwatch.ms(), this->time * time_limit_rate)).draw(Arg::center(1280 / 2, 40 / 2), Palette::Black);
	//}
}
