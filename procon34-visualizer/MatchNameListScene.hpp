# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"


class MatchNameListScene : public App::Scene{
public:
	MatchNameListScene(const InitData &init);
	void update(void) override;
	void draw(void) const override;
private:
	LoadMatches loadmatches;
	Array<String> st;
};

