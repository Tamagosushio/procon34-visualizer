# pragma once
# include <Siv3D.hpp>
# include "LoadMatches.hpp"
# include "Data.hpp"


Data::Data() {}

void Data::set_match_name(const String& str) {
	this->match_name = str;
}
String Data::get_match_name(void)const {
	return this->match_name;
}

void Data::set_match_id(const int& id) {
	this->match_id = id;
}
int Data::get_match_id(void) const {
	return this->match_id;
}

void Data::set_field_type(const String& str) {
	this->field_type = str;
}
String Data::get_field_type(void) const {
	return this->field_type;
}

LoadMatches& Data::get_loadmatches(void) {
	return this->loadmatches;
}

