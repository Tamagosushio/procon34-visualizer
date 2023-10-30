# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Data.hpp"
# include "MatchNameListScene.hpp"
# include "MatchListScene.hpp"
# include "FieldScene.hpp"

void Main(){

	Window::Resize(450, 1000);
	Window::SetStyle(WindowStyle::Sizable);
	Scene::SetBackground(Palette::Silver);

	App manager;
	manager.add<MatchNameListScene>(U"MatchNameListScene");
	manager.add<MatchListScene>(U"MatchListScene");
	manager.add<FieldScene>(U"FieldScene");

	manager.init(U"MatchNameListScene");


	while (System::Update()){
		if (not manager.update()) {
			break;
		}
	}
}


