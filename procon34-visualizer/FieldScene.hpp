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

	int match_id;
	String field_type;
	int height, width;
	String team_first, team_second;
	int turn_now, turn_limit;
	

	// グリッド上の中心座標、四角形、円を取得
	constexpr Point get_cell_center(const Point& p) const;
	constexpr Rect get_grid_rect(const Point& p) const;
	constexpr Rect get_grid_rect(const int y, const int x) const;
	constexpr Circle get_grid_circle(const Point& p) const;
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
	// フィールド情報の更新
	void update_walls(const Array<Array<int>> &masons);
	void update_territories(const Array<Array<int>> &masons);
	void update_masons(const Array<Array<int>>& masons);
	void update_turn(void);
	// フィールド情報の表示
	void draw_gridlines(void) const;
	void draw_actors(void) const;
	void draw_details(void) const;
	// 盤面情報
	Array<Array<CELL>> grid;
	// 時間経過でターンを進めるためのストップウォッチ
	Stopwatch stopwatch;
	bool is_paused = false;
};


