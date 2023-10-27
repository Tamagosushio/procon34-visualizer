# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"


class MatchListScene : public  App::Scene{
public:
	MatchListScene(const InitData &init);
	void update(void) override;
	void draw(void) const override;
private:
	// SimpleGUIを用いないボタン表示
	void draw_buttons(void) const;
	void update_buttons(void);
	void update_scroll(void);
	void update_button_size(void);

	Rect get_rect_button(const int cnt) const;

	String match_name;
	Array<MatchCSV> matches;

	// スクロールで移動する添え字
	int scroll_idx = 0;
	// ボタンの高さ
	int button_height = 100;
	// ボタンの幅
	int button_width = Scene::Size().x / 3;
	// ボタンの上下間隔
	int button_blank_y = button_height / 3;
	// ボタンの色
	const Color button_color = Color(Palette::Skyblue);
	// ボタンに使うフォント
	Font font_button{ FontMethod::SDF, 50, U"SourceHanSansJP-Medium.otf"};

};


