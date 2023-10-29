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
	this->height = grid.size(); this->width = grid.front().size();
	// 先攻チームと後攻チームの名前
	this->team1 = matchcsv.team1;
	this->team2 = matchcsv.team2;
	// ターン数に関する初期化
	this->turn_num_pre = this->turn_num_now = 0;
	this->turn_num_limit = matchjson.turns.size() - 1;
	this->turns = getData().get_loadmatches().get_match_json(match_id).turns;
	// ターンごとの得点をセット
	for(const Turn &turn: matchjson.turns){
		Array<int> ary;
		for(const Scores &scores: turn.scores){
			int total = scores.wall_score + scores.territory_score + scores.castle_score;
			ary << total;
		}
		points << ary;
	}
	// 初フレームをレスポンシブにする
	update_responsive();
}



void FieldScene::draw(void) const {
	draw_gridlines();
	draw_actors();
	draw_details();
	draw_images();
	draw_graph();
}


void FieldScene::update(void) {
	update_responsive();
	update_field();
	update_gui();
	update_turn();
}
// ターン更新
void FieldScene::update_turn(void) {
	if (not is_paused) {
		// 時間によるターン遷移
		if (not stopwatch.isStarted()) {
			stopwatch.start();
		}
		if (stopwatch.ms() > time_step) {
			turn_num_now = Min(turn_num_now + 1, turn_num_limit);
			stopwatch.restart();
		}
	}
}
// GUI更新
void FieldScene::update_gui(void) {
	// 時間経過による再生の有無
	if (Circle(anchor_play_button, image_radius).leftClicked() or KeySpace.down()) {
		stopwatch.reset();
		is_paused ^= true;
	}
	// 戻るボタン
	if (Circle(anchor_return_button, image_radius).leftClicked()) {
		changeScene(U"MatchListScene", 0s);
	}
	// スライダーによるターン遷移
	double turn_rate = (double)turn_num_now / (double)turn_num_limit;
	if (SimpleGUI::Slider(turn_rate, { 0,0 }, Scene::Size().x)) {
		turn_num_now = turn_rate * turn_num_limit;
		// GUIの操作中は時間経過のターン遷移を行わない
		stopwatch.restart();
	}
	// ボタンによるターン遷移
	if (Circle(anchor_arrow_left, image_radius * 0.75).leftClicked() or KeyLeft.down()) {
		turn_num_now = Max(turn_num_now - 1, 0);
	}
	if (Circle(anchor_arrow_right, image_radius * 0.75).leftClicked() or KeyRight.down()) {
		turn_num_now = Min(turn_num_now + 1, turn_num_limit);
	}
}
// フィールド更新
void FieldScene::update_field() {
	if (turn_num_pre != turn_num_now) {
		update_walls(turns[turn_num_now].board.walls);
		update_territories(turns[turn_num_now].board.territories);
		update_masons(turns[turn_num_now].board.masons);
		turn_num_pre = turn_num_now;
	}
}
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
			Shape2D::Star(cell_size * 0.6, get_cell_center(p)).draw(Palette::Black);
		}
		if (target_cell & CELL::AREA_RED and target_cell & CELL::AREA_BLUE) {
			get_grid_rect(p).draw(ColorF(1.0, 0.0, 1.0, 0.5));
		}else 	if (target_cell & CELL::AREA_RED) {
			get_grid_rect(p).draw(ColorF(1.0, 0.0, 0.0, 0.25));
		}else 	if (target_cell & CELL::AREA_BLUE) {
			get_grid_rect(p).draw(ColorF(0.0, 0.0, 1.0, 0.25));
		}
		if (target_cell & CELL::WALL_RED) {
			Rect(Arg::center(get_cell_center(p)), (int)(cell_size * 0.7)).draw(Palette::Red);
		}
		if (target_cell & CELL::WALL_BLUE) {
			Rect(Arg::center(get_cell_center(p)), (int)(cell_size * 0.7)).draw(Palette::Blue);
		}
		if (target_cell & CELL::CRAFTSMAN_RED) {
			Circle(Arg::center(get_cell_center(p)), cell_size * 0.3).draw(ColorF(1.0, 0.5, 0.5));
			Circle(Arg::center(get_cell_center(p)), cell_size * 0.3).drawFrame(1, 1, Palette::White);
		}
		if (target_cell & CELL::CRAFTSMAN_BLUE) {
			Circle(Arg::center(get_cell_center(p)), cell_size * 0.3).draw(ColorF(0.5, 0.5, 1.0));
			Circle(Arg::center(get_cell_center(p)), cell_size * 0.3).drawFrame(1, 1, Palette::White);
		}
	}
}
void FieldScene::draw_details(void) const {
	Scores scores_first = turns[turn_num_now].scores[0];
	Scores scores_second = turns[turn_num_now].scores[1];

	int x = Scene::Center().x + cell_size*5;
	font_details(U"{}: {}pt"_fmt(team1, scores_first.wall_score + scores_first.territory_score + scores_first.castle_score))
		.draw(font_size, x, blank_top, Palette::Red);
	font_details(U"城壁:{}  陣地:{}  城:{}"_fmt(scores_first.wall_score/10, scores_first.territory_score/30, scores_first.castle_score/100))
		.draw(font_size/2, x + cell_size, blank_top+font_size+cell_size/2, Palette::Black);
	font_details(U"{}: {}pt"_fmt(team2, scores_second.wall_score + scores_second.territory_score + scores_second.castle_score))
		.draw(font_size, x, blank_top + (font_size+cell_size) * 1.5, Palette::Blue);
	font_details(U"城壁:{}  陣地:{}  城:{}"_fmt(scores_second.wall_score / 10, scores_second.territory_score / 30, scores_second.castle_score / 100))
		.draw(font_size / 2, x + cell_size, blank_top + font_size*2.5 + cell_size*2.0, Palette::Black);
	font_details(U"ターン数: {}/{}"_fmt(turn_num_now, turn_num_limit))
		.draw(font_size, x, blank_top + (font_size+cell_size) * 3.0, Palette::Black);

}
void FieldScene::draw_images(void) const {
	// 再生 /  停止ボタンの描画
	(is_paused ? image_play : image_stop).resized(image_radius*2).draw(anchor_play_button);
	// 戻るボタン
	image_return.resized(image_radius*2).draw(anchor_return_button);
	// 矢印
	image_arrow_left.resized(image_radius * 1.5).draw(anchor_arrow_left);
	image_arrow_right.resized(image_radius * 1.5).draw(anchor_arrow_right);
}
void FieldScene::draw_graph() const {
	// 背景
	RectF background{ graph_lefttop, graph_size };
	// 範囲内での得点の最大値を求める
	int max_point = 0;
	for(int idx = 0; idx <= turn_num_now; idx++){
		for (const int &num : points[idx]) {
			max_point = Max(max_point, num);
		}
	}
	// 範囲内でのターンごとの得点の差の最大値を求める
	Array<std::pair<int, int>> max_diff = { { 0,0 }, {0,0} };
	for (int idx = 0; idx <= turn_num_now - 1; idx++) {
		for (int team = 0; team <= 1; team++) {
			if (Abs(points[idx][team] - points[idx + 1][team]) > max_diff[team].first) {
				max_diff[team] = {Abs(points[idx][team] - points[idx + 1][team]), idx};
			}
		}
	}
	// 各ターン各チームの得点に対する座標を計算
	Array<Array<Vec2>> coordinates;
	for(int idx = 0; idx <= turn_num_now; idx++){
		Array<Vec2> ary;
		for(const int &num : points[idx]){
			const double x = background.leftX() + background.w * (double)idx / (double)turn_num_now;
			const double y = background.topY() + background.h - background.h * (double)num / (double)max_point;
			ary << Vec2(x, y);
		}
		coordinates << ary;
	}
	// 座標から線分で結ぶ
	for(int idx = 0; idx <= turn_num_now -1; idx++){
		Line{ coordinates[idx][0], coordinates[idx + 1][0] }.draw(graph_line_thick, Palette::Red);
		Line{ coordinates[idx][1], coordinates[idx + 1][1] }.draw(graph_line_thick, Palette::Blue);
	}
	// 縦軸の目盛りを描画
	for (int i = 0; i < graph_scale; i++) {
		const double y = background.topY() + graph_size.y * i / graph_scale ;
		const int scale = (int)(max_point * ((double)(graph_scale - i) / (double(graph_scale))));
		font_details(scale).draw(cell_size, Arg::rightCenter(background.leftX() - cell_size / 2, y), Palette::Black);
		Line{ background.rightX(), y, background.leftX(), y}.draw(graph_line_thick / 2, graph_scale_color);
	}
	// 横軸の目盛りを描画
	for (int team = 0; team <= 1; team++) {
		const double x = background.leftX() + background.w * (double)max_diff[team].second / (double)turn_num_now;
		const double y = background.bottomY() + cell_size * team;
		Line{ x, y, x, background.topY() }.draw(graph_line_thick / 2, graph_scale_color);
		font_details(max_diff[team].second).draw(cell_size, Arg::topCenter(x, y), Palette::Black);
	}
	// 縦軸横軸の矢印を描画
	background.left().drawArrow(graph_line_thick * 2, Vec2{cell_size, cell_size}, Palette::Black);
	background.bottom().reverse().drawArrow(graph_line_thick * 2, Vec2{cell_size, cell_size}, Palette::Black);
}


void FieldScene::update_responsive(void) {
	this->cell_size = Min(Scene::Center().x / (width + 6), Scene::Size().y * 2 / 3 / height);
	this->blank_left = cell_size * 2;
	this->image_radius = cell_size;
	this->font_size = cell_size * height / 2 / 5;
	this->anchor_play_button = Arg::topCenter(Scene::Center().x, blank_top);
	this->anchor_return_button = Arg::bottomLeft(cell_size, Scene::Size().y - cell_size);
	this->anchor_arrow_left = Arg::rightCenter(Scene::Center().x - image_radius*2, blank_top + image_radius);
	this->anchor_arrow_right = Arg::leftCenter(Scene::Center().x + image_radius*2, blank_top + image_radius);
	this->graph_lefttop = Vec2(Scene::Center().x + cell_size*2, cell_size * height * 0.75 + cell_size);
	this->graph_size = Size(Scene::Size().x - (int)graph_lefttop.x - cell_size, Scene::Size().y - (int)graph_lefttop.y - cell_size*3);
}

// フィールドの座標を引数に、セルの中心の画面上の座標を返す
const Point FieldScene::get_cell_center(const Point& p)const {
	return Point(p.x * cell_size + blank_left + cell_size / 2, p.y * cell_size + blank_top + cell_size / 2);
}
const Rect FieldScene::get_grid_rect(const Point& p)const {
	return Rect{ p.x * cell_size + blank_left, p.y * cell_size + blank_top, cell_size };
}
const Rect FieldScene::get_grid_rect(const int y, const int x)const {
	return Rect{ x * cell_size + blank_left, y * cell_size + blank_top, cell_size };
}
const Circle FieldScene::get_grid_circle(const Point& p)const {
	return Circle(Arg::center(get_cell_center(p)), cell_size * 0.3);
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


