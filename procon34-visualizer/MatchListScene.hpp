# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "LoadMatches.hpp"


class MatchListScene : public  App::Scene{
public:
	MatchListScene(const InitData &init);
	void update(void) override;
	void draw(void) const override;
private:
	// SimpleGUIを用いないボタン表示
	void draw_buttons(void) const;
	// マウスによるスクロール
	void update_scroll(void);
	// メニュータブの表示と更新
	void update_menutab(void);

	// スクロールで移動する添え字
	int scroll_idx = 0;
	// メニュータブの高さ
	const int menutab_height = 100;
	// ボタンの高さ
	const int button_height = 60;
	// ボタンの幅
	int button_width = Scene::Size().x / 3;
	//const int button_blank_y = 10;
	// ボタンの色
	const Color button_color = Color(Palette::Skyblue).setA(128);
	// メニュータブに使うフォント
	Font font_menutab{75, U"SourceHanSansJP-Medium.otf" };
	// ボタンに使うフォント
	Font font_button{50, U"SourceHanSansJP-Medium.otf"}; 

	HashTable<String, Array<MatchCSV>> hashtable;

	LoadMatches loadmatches;
};


