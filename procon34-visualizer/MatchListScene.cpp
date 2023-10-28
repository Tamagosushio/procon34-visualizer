# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"
# include "MatchListScene.hpp"

MatchListScene::MatchListScene(const InitData &init) :IScene(init) {
	this->match_name = getData().get_match_name();
	for (const MatchCSV &matchcsv : getData().get_loadmatches().get_match_csv()) {
		if (matchcsv.match_name == this->match_name) {
			matches << matchcsv;
		}
	}
	update();
}

void MatchListScene::update(void){
	update_responsive();
	update_buttons();
	update_scroll();
}

void MatchListScene::draw(void)const {
	draw_images();
	draw_buttons();
}

void MatchListScene::update_buttons(void) {
	int cnt = 0;
	for (const MatchCSV& matchcsv : matches) {
		Rect button_rect = get_rect_button(cnt);
		if (button_rect.mouseOver()) {
			Cursor::SetDefaultStyle(CursorStyle::Hand);
			if (button_rect.leftClicked()) {
				getData().set_match_id(matchcsv.match_id);
				getData().set_field_type(matchcsv.field_type);
				changeScene(U"FieldScene");
			}
		}
		cnt++;
	}
	if (Circle(anchor_return_button, image_radius).leftClicked()) {
		changeScene(U"MatchNameListScene", 0s);
	}
}
void MatchListScene::update_scroll(void) {
	// スクロールは上下で制限をかける
	if (Mouse::Wheel() > 0) {
		double numerator = (double)button_height * ((double)matches.size() / 2.0) + (double)button_blank_y * ((double)matches.size() / 2.0 + 1.0) - (double)Scene::Size().y;
		double denominator = (double)button_height + (double)button_blank_y;
		scroll_idx = Max<int>(Min<int>(scroll_idx + 1, Math::Ceil(numerator / denominator)), 0);
	}else if (Mouse::Wheel() < 0) {
		scroll_idx = Max<int>(scroll_idx - 1, 0);
	}
}
void MatchListScene::update_responsive(void) {
	this->button_width = Scene::Size().x / 3;
	this->button_height = Scene::Size().y / 10;
	this->button_blank_y = button_height / 3;
	this->anchor_return_button = Arg::bottomLeft(button_height, Scene::Size().y - button_height);
	this->image_radius = button_height / 2;
}

void MatchListScene::draw_buttons(void) const {
	int cnt = 0;
	for(const MatchCSV &matchcsv : matches ){
		Rect button_rect = get_rect_button(cnt);
		button_rect.draw(button_color).drawFrame(1, 1, Palette::Black);
		const String button_lebel = U"{} VS {}"_fmt(matchcsv.team1, matchcsv.team2);
		// ボタンの四角形の中に収まる最大のフォントサイズを二分探索
		int left = 0, right = 1000;
		while (Abs(right - left) > 1) {
			int mid = (left + right) / 2;
			if (font_button(button_lebel).draw(mid, button_rect.stretched(-5), HSV(0,0,0,0))) {
				left = mid;
			}else {
				right = mid;
			}
		}
		font_button(button_lebel).draw(left, button_rect.stretched(-5), Palette::Black);
		cnt++;
	}
}
void MatchListScene::draw_images(void)const {
	// 戻るボタン
	image_return.resized(image_radius*2).draw(anchor_return_button);
}

Rect MatchListScene::get_rect_button(const int cnt) const {
	int x = Scene::Center().x;
	int y = button_height * ((cnt / 2)) + button_blank_y * ((cnt / 2) + 1) - scroll_idx * (button_height + button_blank_y);
	if (cnt % 2 == 0) {
		return Rect{ Arg::topRight(x,y), button_width, button_height };
	}
	else {
		return Rect{ Arg::topLeft(x,y), button_width, button_height };
	}
}



