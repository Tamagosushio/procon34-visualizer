# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Base.hpp"


template <class T>
Array<T> get_1d_array(const JSON& json) {
	Array<T> res;
	for (const auto& num : json.arrayView()) {
		res << num;
	}
	return res;
}
template <class T>
Array<Array<T>> get_2d_array(const JSON& json) {
	Array<Array<T>> res;
	for (const auto& object : json.arrayView()) {
		Array<T> ary;
		for (const auto& num : object.arrayView()) {
			ary << num.get<int>();
		}
		res << ary;
	}
	return res;
}


struct MatchCSV {
	String match_name;
	String field;
	String team1;
	String team2;
	int match_id;
};

struct Scores {
	int wall_score;
	int territory_score;
	int castle_score;
	Scores(const JSON &json){
		this->wall_score = json[U"wall_score"].get<int>();
		this->territory_score = json[U"territory_score"].get<int>();
		this->castle_score = json[U"castle_score"].get<int>();
	}
};
struct Board {
	Array<Array<int>> masons;
	Array<Array<int>> walls;
	Array<Array<int>> territories;
	Board(const JSON &json){
		this->masons = get_2d_array<int>(json[U"masons"]);
		this->walls = get_2d_array<int>(json[U"walls"]);
		this->territories = get_2d_array<int>(json[U"territores"]);
	}
};
struct Turn {
	int turn;
	Board board;
	Scores scores;
	Turn(const JSON &json){
		this->turn = json[U"turn"].get<int>();
		this->board = json[U"board"].get<Board>();
		this->scores = json[U"scores"].get<Scores>();
	}
};
struct MatchJson {
	int match_id;
	Array<Turn> turns;
	MatchJson(const JSON &json){
		this->match_id = json[U"match_id"].get<int>();
		this->turns = get_1d_array<Turn>(json[U"turns"]);
	}
};

class LoadMatches {
public:
	LoadMatches(void);
	Array<MatchCSV>& get_match_csv(void);
	Optional<MatchCSV> get_match_csv(const String& id);
private:
	// 試合情報のcsvから読み込み
	void load_matches_csv(void);
	const String match_csv_path = U"./matches.csv";
	Array<MatchCSV> ary_match_csv;
	// フィールド情報のcsvから読み込み
	void load_field_csv(void);
	const String field_csv_path = U"./field/";
	// 試合情報のjsonから読み込み
	void load_match_json(void);
	const String match_json_path = U"./match_jsons/";
	Array<MatchJson> ary_match_json;
};
