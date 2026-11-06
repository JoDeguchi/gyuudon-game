#pragma once

#include "GameState.h"
#include "KeyReader.h"
#include "Graph.h"
#include "Steam.h"
#include <vector>

class TitleState :public GameState
{
private:

	KeyReader key;
	GraphDraw bg;
	std::vector<Steam> steam;
	GraphDraw EnterBotan;
	GraphDraw SettingBotan;

public:

	int mode = 0;		//	モード切り替え用変数
	int fontHandle = 0;	//	フォント変数

	TitleState(GameManager* mgr) : GameState(mgr) {}

	void Init() override;
	void Input() override;
	void Update()override;
	void Draw()override;
};