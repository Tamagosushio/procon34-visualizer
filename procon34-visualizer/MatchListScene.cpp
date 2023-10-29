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
	update_responsive();
}

void MatchListScene::update(void){
	update_responsive();
	update_buttons();
	update_scroll();
	if(frame_cnt <= 1) frame_cnt++;
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
			if (frame_cnt > 1 and button_rect.leftReleased()) {
				getData().set_match_id(matchcsv.match_id);
				getData().set_field_type(matchcsv.field_type);
				changeScene(U"FieldScene", 0s);
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
	this->anchor_return_button = Arg::bottomLeft(button_blank_y, Scene::Size().y - button_blank_y);
	this->image_radius = button_height / 2;
}

void MatchListScene::draw_buttons(void) const {
	int cnt = 0;
	for(const MatchCSV &matchcsv : matches ){
		Rect button_rect = get_rect_button(cnt);
		button_rect = draw_button_rect(button_rect, button_color, button_shadow_color);
		const String button_label = ((matchcsv.match_id % 2 == 1) ? U"(先){} VS {}(後)"_fmt : U"(後){} VS {}(先)"_fmt)(matchcsv.team1, matchcsv.team2);
		draw_button_label(button_label, button_rect, font_button, Palette::White);
		cnt++;
	}
}
void MatchListScene::draw_images(void)const {
	// 戻るボタン
	image_return.resized(image_radius*2).draw(anchor_return_button);
}

Rect MatchListScene::get_rect_button(const int cnt) const {
	int x = Scene::Center().x;
	int blank = Scene::Size().x / 25;
	int y = button_height * ((cnt / 2)) + button_blank_y * ((cnt / 2) + 1) - scroll_idx * (button_height + button_blank_y);
	if (cnt % 2 == 0) {
		return Rect{ Arg::topRight(x-blank, y), button_width, button_height };
	}else {
		return Rect{ Arg::topLeft(x+blank, y), button_width, button_height };
	}
}



