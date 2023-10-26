# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Base.hpp"
# include "MatchListScene.hpp"
# include "Field.hpp"
# include "LoadMatches.hpp"

void Main(){

	Window::Resize(1280, 720);
	Window::SetStyle(WindowStyle::Frameless);
	Scene::SetBackground(Palette::Gray);
	LoadMatches loadmatches;

	App manager;
	manager.add<MatchListScene>(U"ListScene");


	while (System::Update()){
		if (not manager.update()) {
			break;
		}
	}
}


