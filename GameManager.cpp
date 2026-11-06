#include "GameManager.h"
#include "GameState.h"

GameManager* GameManager::instance = nullptr;

//	ステート変える
void GameManager::ChangeState(std::unique_ptr<GameState> next)
{
	currentState = std::move(next);
	currentState->Init();
}


void GameManager::GameLoop()
{
	PlayMusic("data/bgm.mp3", DX_PLAYTYPE_LOOP);
	SetVolumeMusic(200);
	// ループ開始
	while (ProcessMessage() == 0)
	{
		// リフレッシュレートを設定するための処理
		clock_t check_fps = clock() + CLOCKS_PER_SEC / 60;

		// マウスカーソル表示設定
		SetMouseDispFlag(FALSE);

		// 画面上の描画を初期化（画面を消去）
		ClearDrawScreen();

		//	入力処理
		currentState->Input();
		//	更新処理
		currentState->Update();
		//	描画処理
		currentState->Draw();

		if(CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		

		// リフレッシュレートが一定になるまで待つ処理
		while (clock() < check_fps) {}

		// 裏画面の描画を表に反映
		ScreenFlip();
	}
}
