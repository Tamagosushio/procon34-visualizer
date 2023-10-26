# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"
# include "MatchNameListScene.hpp"


MatchNameListScene::MatchNameListScene(const InitData& init) : IScene(init) {
	for (const MatchCSV& matchcsv : loadmatches.get_match_csv()) {
		st << matchcsv.match_name;
	}
	st.unique_consecutive();
}

void MatchNameListScene::update(void) {
	int cnt = 1;
	for (auto iter = st.begin(); iter != st.end(); iter++, cnt++) {
		if (SimpleGUI::Button(*iter, { 50, 50 * cnt })) {
			getData().set_match_name(*iter);
			changeScene(U"MatchListScene");
		}
	}
}

void MatchNameListScene::draw(void)const {

}

