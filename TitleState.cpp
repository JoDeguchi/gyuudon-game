#include "TitleState.h"
#include "GameManager.h"
#include "StartGameState.h"

//	モード選択する枠の位置
#define SETTING_X 0
#define SETTING_Y 400

//	初期化
void TitleState::Init()
{
	//	背景
	bg=GraphDraw("data/bg_title.png", 0, 0);
	//	湯気
	steam.push_back(Steam("data/steam.png", 370, 250));
	steam.push_back(Steam("data/steam.png", 300, 350));
	steam.push_back(Steam("data/steam.png", 600, 300));
	steam.push_back(Steam("data/steam.png", 850, 350));

	//	エンターボタン
	EnterBotan = GraphDraw("data/PressEnter.png", 400, 500);
	//	フォント変える
	//	フォントの読み込み
	AddFontResourceExA("data/TanukiMagic.ttf", FR_PRIVATE, NULL);
	fontHandle = CreateFontToHandle(
		"たぬき油性マジック",
		40,
		10,
		DX_FONTTYPE_ANTIALIASING
	);
}
//	入力処理
void TitleState::Input()
{
	key.Read();	//	キーの読み込み

	//	エンターキーでゲーム画面へ
	if (key.CheckReleaseKey(KEY_INPUT_RETURN)) {

		manager->ChangeState(
			std::make_unique<StartGameState>(manager)
		);
	}

	//	上下キーで選択できるように
	if (key.CheckPressKey(KEY_INPUT_UP)){
		mode--;
		if (mode < 0)	mode = 1;
	}
	if (key.CheckPressKey(KEY_INPUT_DOWN)){
		mode++;
		if (mode > 1)	mode = 0;

	}

}
//	更新処理
void TitleState::Update()
{
	//	湯気の描画
	for (auto& s : steam)
	{
		s.Update();
	}
}

//	描画処理
void TitleState::Draw()
{
	bg.Draw();				//	背景
	EnterBotan.Draw();		//Enterボタンの描画
	
	//	湯気の描画
	for (const auto& s : steam){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
		s.Draw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

