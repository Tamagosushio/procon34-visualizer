# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "LoadMatches.hpp"


LoadMatches::LoadMatches(void) {
	this->load_matches_csv();
}

// -----------------------------------------------------------------------------------------------
// 対戦ログの末尾が1で終わっているものが第1対戦の結果、
// 末尾が2で終わっているものが第2対戦の結果です。
// 第1対戦では、team1が先攻、第2対戦ではteam2が先攻になります。
// 試合名, フィールド, team1, team2, match_id
// -----------------------------------------------------------------------------------------------
Array<MatchCSV>& LoadMatches::get_match_csv(void) {
	return this->ary_match_csv;
}
Optional<MatchCSV> LoadMatches::get_match_csv(const String &id){
	for(const MatchCSV &matchcsv: ary_match_csv){
		if (matchcsv.match_id == id) {
			return matchcsv;
		}
	}
	return none;
}
void LoadMatches::load_matches_csv(void) {
	const CSV csv{ match_csv_path };
	if (not csv) {
		throw Error{ U"Failed to load " + match_csv_path };
	}
	for (int row = 0; row < csv.rows(); row++) {
		MatchCSV matchcsv;
		matchcsv.match_name = csv[row][0];
		matchcsv.field = csv[row][1];
		matchcsv.team1= csv[row][2];
		matchcsv.team2= csv[row][3];
		matchcsv.match_id = Parse<int>(csv[row][4]);
		ary_match_csv << matchcsv;
	}
}





