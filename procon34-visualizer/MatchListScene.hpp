# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"


class MatchListScene : public  App::Scene{
public:
	MatchListScene(const InitData &init);
	void update(void) override;
	void draw(void) const override;
private:
	void update_buttons(void);
	void update_scroll(void);
	void update_responsive(void);

	// SimpleGUIを用いないボタン表示
	void draw_buttons(void) const;
	void draw_images(void) const;

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
	Font font_button{ FontMethod::SDF, 50, Resource(U"SourceHanSansJP-Medium.otf") };
	// 戻るボタン
	int image_radius = button_height / 2;
	Arg::bottomLeft_<Vec2> anchor_return_button;
	const Texture image_return = Texture(Resource(U"./images/return.png"));
	// シーンが開始されてから数フレームはマウスのクリック関連に不具合がある？
	int frame_cnt = 0;
};


