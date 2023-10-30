# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"


class MatchNameListScene : public App::Scene{
public:
	MatchNameListScene(const InitData &init);
	void update(void) override;
	void draw(void) const override;
private:
	void update_responsive(void);
	int button_height;
	int button_width;
	int button_blank;
	LoadMatches loadmatches;
	Array<String> st;\
	Font font_button{ FontMethod::SDF, 50, Resource(U"SourceHanSansJP-Medium.otf") };
	// シーンが開始されてから数フレームはマウスのクリック関連に不具合がある？
	int frame_cnt = 0;
};

