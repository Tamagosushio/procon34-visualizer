# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"





class FieldScene : public App::Scene{
public:
	FieldScene(const InitData &init);
	void update(void) override;
	void draw(void) const override;
private:
	// 自作csvファイルからの試合情報
	MatchCSV matchcsv;
	// 公開されてるjsonファイルからの試合情報
	MatchJSON matchjson;
	// 現在のターンの試合情報
	Array<Turn> turns;
	
	int match_id;
	String field_type;
	int height, width;
	String team1, team2;
	int turn_num_pre, turn_num_now, turn_num_limit;
	
	// フィールド更新
	void update_field(void);
	void update_turn(void);
	void update_gui(void);
	void update_walls(const Array<Array<int>> &masons);
	void update_territories(const Array<Array<int>> &masons);
	void update_masons(const Array<Array<int>>& masons);
	void update_responsive(void);
	// フィールド表示
	void draw_gridlines(void) const;
	void draw_actors(void) const;
	void draw_details(void) const;
	void draw_images(void) const;


	// 盤面情報
	Array<Array<CELL>> grid;
	// 時間経過でターンを進めるためのストップウォッチ
	Stopwatch stopwatch;
	bool is_paused = false;
	int time_step = 1000;
	double speed_time_step = 1.0;
	// 一つのセルの大きさ(正方形)
	int cell_size = 20;
	// フィールドの左上に開ける余白
	int blank_left = 50;
	int blank_top = 50;
	// 再生、停止ボタンの画像
	const Texture image_play = Texture(U"./images/再生ボタン.png");
	const Texture image_stop = Texture(U"./images/一時停止ボタン.png");
	const Texture image_return = Texture(U"./images/Uターン矢印.png");
	int image_radius = cell_size * 2;
	// 詳細表示のフォント
	int font_size = 100;
	const Font font_details = Font(FontMethod::SDF, font_size, U"SourceHanSansJP-Medium.otf");
	Arg::topCenter_<Vec2> anchor_play_button;
	Arg::bottomLeft_<Vec2> anchor_return_button;

	// グリッド上の中心座標、四角形、円を取得
	const Point get_cell_center(const Point& p) const;
	const Rect get_grid_rect(const Point& p) const;
	const Rect get_grid_rect(const int y, const int x) const;
	const Circle get_grid_circle(const Point& p) const;
	// グリッド上の座標がグリッドに収まっているか
	bool is_in_field(const int y, const int x) const;
	bool is_in_field(const Point& p) const;
	// セルの情報を取得する
	CELL get_cell(const int y, const int x) const;
	CELL get_cell(const Point p) const;
	// セルの情報を追加する
	void set_bit(const int y, const int x, const CELL new_bit);
	void set_bit(const Point p, const CELL new_bit);
	// セルの情報を削除する
	void delete_bit(const int y, const int x, const CELL delete_bit);
	void delete_bit(const Point p, const CELL delete_bit);
};


