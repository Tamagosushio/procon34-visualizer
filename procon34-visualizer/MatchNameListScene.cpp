# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"
# include "MatchNameListScene.hpp"


MatchNameListScene::MatchNameListScene(const InitData& init) : IScene(init) {
	for (const MatchCSV& matchcsv : loadmatches.get_match_csv()) {
		st << matchcsv.match_name;
	}
	st.unique_consecutive();
	update_responsive();
}

void MatchNameListScene::update(void) {
	int cnt = 0;
	update_responsive();
	for (auto iter = st.begin(); iter != st.end(); iter++, cnt++) {
			Rect rect{ Arg::topCenter(Scene::Center().x, button_blank * (cnt + 1) + button_height * cnt), Size{button_width, button_height} };
			if (rect.leftReleased()) {
				getData().set_match_name(*iter);
				changeScene(U"MatchListScene", 0s);
			}
	}
}

void MatchNameListScene:: update_responsive(void) {
	this->button_height = Scene::Size().y / (st.size() * 1.5);
	this->button_width = Scene::Size().x / 2;
	this->button_blank = button_height / 2;
}

void MatchNameListScene::draw(void)const {
	int cnt = 0;
	for (auto iter = st.begin(); iter != st.end(); iter++, cnt++){
		Rect rect{ Arg::topCenter(Scene::Center().x, button_blank * (cnt + 1) + button_height * cnt), Size{button_width, button_height} };
		HSV hsv{ 360 * cnt / st.size(), 0.6, 0.85 };
		HSV hsv_shadow{ 360 * cnt / st.size(), 0.6, 0.6 };
		rect = draw_button_rect(rect, hsv, hsv_shadow);
		draw_button_label(*iter, rect, font_button, Palette::White, true);
	}
}

