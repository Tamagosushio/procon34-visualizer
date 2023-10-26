# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "MatchListScene.hpp"
# include "LoadMatches.hpp"

MatchListScene::MatchListScene(const InitData &init) :IScene(init) {
	for(const MatchCSV &matchcsv: getData().get_match_csv()){
		hashtable[matchcsv.match_name] << matchcsv;
	}
}

void MatchListScene::update(void){
	update_scroll();
}

void MatchListScene::draw(void)const {
	draw_buttons();
}

void MatchListScene::update_scroll(void) {
}

void MatchListScene::draw_buttons(void) const {
	int cnt = 0;	
	for (const MatchCSV& matchcsv : getData().get_match_csv()) {
		int x = Scene::Center().x;
		int y = menutab_height + (button_height) * (cnt / 2);
		Rect rect_button;
		if (cnt % 2 == 0) {
			rect_button = Rect(Arg::topRight(x, y), button_width, button_height);
		}else{
			rect_button = Rect(Arg::topLeft(x, y), button_width, button_height);
		}
		rect_button.draw(button_color).drawFrame(1, 1, Palette::Black);
		font_button(U"{}  VS  {}"_fmt(matchcsv.team1, matchcsv.team2)).draw(button_height/2, rect_button.stretched(-5), Palette::Black);
		if (rect_button.mouseOver()) {
			Cursor::SetDefaultStyle(CursorStyle::Hand);
			if (rect_button.leftClicked()) {

			}
		}
		cnt++;
	}
}

void MatchListScene::update_menutab(void) {
	for (auto iter = hashtable.begin(); iter != hashtable.end(); iter++) {
		
	}
}




