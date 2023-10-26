# pragma once
# include <Siv3D.hpp>
# include "LoadMatches.hpp"


class Data {
public:
	Data();
	void set_match_name(const String& str);
	String get_match_name(void) const;
	void set_match_id(const int &id);
	int get_match_id(void) const;
	void set_field_type(const String &str);
	String get_field_type(void) const;
	LoadMatches& get_loadmatches(void) ;
private:
	String match_name;
	int match_id;
	String field_type;
	LoadMatches loadmatches;
};

using App = SceneManager<String, Data>;
