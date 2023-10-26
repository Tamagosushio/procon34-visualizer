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
}

void MatchListScene::update(void){
	update_buttons();
	update_scroll();
}

void MatchListScene::draw(void)const {
	draw_buttons();
}


void MatchListScene::update_scroll(void) {
}

Rect MatchListScene::get_rect_button(const int cnt) const {
	int x = Scene::Center().x;
	int y = button_height * (cnt/2)+button_blank_y * (cnt /2+ 1);
	if (cnt % 2 == 0) {
		return Rect{ Arg::topRight(x,y), button_width, button_height };
	}else {
		return Rect{ Arg::topLeft(x,y), button_width, button_height };
	}
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
}

void MatchListScene::draw_buttons(void) const {
	int cnt = 0;
	for(const MatchCSV &matchcsv : matches ){
		Rect button_rect = get_rect_button(cnt);
		button_rect.draw(button_color).drawFrame(1, 1, Palette::Black);
		cnt++;
	}
}





