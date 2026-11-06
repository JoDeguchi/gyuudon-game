#include "StartGameState.h"
#include "TitleState.h"
#include "GameManager.h"

//	defineとちがってnamespaceは安全に型チェックできる
namespace {
	constexpr float MANUAL_SHOW_X = 270;	//	マニュアル表示するときのX
	constexpr float MANUAL_SHOW_Y = 100;	//	マニュアル表示するときのY
	constexpr float MANUAL_HIDE_Y = 720;	//	隠すときのY(画面の下)
	constexpr int comment_end_x = 880;		//	店長の最終コメントX
	constexpr int comment_end_y = 100;		//　店長の最終コメントY

	constexpr int SERVE_MAX = 5;			//	提供数

}

//	初期化
void StartGameState::Init()
{
	bg = GraphDraw("data/bg.png", 0, 0);			//	背景
	pot = GraphDraw("data/nabe.jpg", 230, 0);		//	鍋
	soup = GraphDraw("data/soup.png", 350, 170);	//	たれ
	//	牛肉の初期化
	for (int i = 0; i < BEEF_NUM; i++){
		beef[i].Load("data/beef.png");
		beef[i].GetRandomF(370, 870, 190, 670);
		beef[i].SetRandomAngle();
	}
	//	たまねぎの初期化
	for (int i = 0; i < ONION_NUM; i++){
		onion[i].Load("data/onion.png");
		//	たまねぎはちょっと大きいので数値小さめ
		onion[i].GetRandomF(370, 810, 170, 650);
		onion[i].SetRandomAngle();
	}
	//	お玉２種類ずつ　柄とカップ
	cup = Radle("data/cup.png", "data/cup2.png", 900, 500);
	cup.SetHitMargin(10, 100, 0, 100);	//	あたり判定微調整
	handle = Radle("data/handle.png", "data/handle2.png", 1070, 630);
	//	柄の左の時のずれ補正　柄だけずれている
	handle.OffsetLeft(-20, -200);

	//	ごはん
	rice = Rice("data/rice.png", 20, 490);
	//	窓口
	window = GraphDraw("data/window.png", -40, -30);
	//	客
	customer= GraphDraw("data/grandfather.png", 40, 20);
	customer2 = GraphDraw("data/grandfather_end.png", 500, 200);
	//	客コメント
	comment = GraphDraw("data/comment.png", 180, -70);
	comment2 = GraphDraw("data/comment2.png", comment_end_x, comment_end_y);
	//	説明書きボード
	explain=GraphDraw("data/board.png", 1020, 10);
	//	ｍキーで押すボタン画像
	mButton = GraphDraw("data/mbottan.png", 990, 350);
	//	Rキーで押すボタン画像
	rButton = GraphDraw("data/Retry.png", 10,560);
	//	具材を数える用のボード画像
	countBoard = GraphDraw("data/count.png", -10, 340);
	//	並牛丼オーダーUI
	order_normal = GraphDraw("data/order_normal.jpg", 0, 220);
	order_large = GraphDraw("data/order_large.jpg", 0, 220);
	//	マニュアル
	manual= GraphDraw("data/manual.jpg", MANUAL_SHOW_X, 100);
	manual_y = MANUAL_HIDE_Y;
	pushMKey = false;

	//	湯気
	steam.push_back(Steam("data/steam.png", 370, 250));
	steam.push_back(Steam("data/steam.png", 300, 430));
	steam.push_back(Steam("data/steam.png", 600, 300));
	steam.push_back(Steam("data/steam.png", 700, 500));
	steam.push_back(Steam("data/steam.png", 650, 150));
	steam.push_back(Steam("data/steam.png", 500, 470));
	steam.push_back(Steam("data/steam.png", 400, 150));

	//	最初の注文をランダムに決める
	order_size = GetRand(1);
	serve_count = 0;

	start_timer = 0;

}
//	入力処理
void StartGameState::Input()
{
	key.Read();	//	キーの読み込み
}
//	更新処理
void StartGameState::Update()
{
	//	開始メッセージ用タイマー
	if (start_timer < START_MSG_TIME) start_timer++;

	//-----------------------------------------------------
	// やること１　お玉で具材を引き上げる

	//	１：お玉を動かす（WASD：移動　　左右キー：浮き沈み）
	//	プレイモードの時のみ
	if (phase == PLAY) {
		cup.Update();
		handle.Update();
	}
	
	cup.Range();	//	お玉移動であたり判定も移動

	//	左キーのときは、すくい状態をリセット
	if (cup.IsLeft()) {
		for (int i = 0; i < BEEF_NUM; i++)   beef[i].isScoop = false;
		for (int i = 0; i < ONION_NUM; i++)  onion[i].isScoop = false;
	}

	//	２：お玉を引き上げたら判定内の具材をすくう
	if (cup.IsScoopUp()) {
		//	先にカップ内の具材の数を数えて、多いときは広めにばらけさせる
		int beef_in = 0;
		for (int i = 0; i < BEEF_NUM; i++) {
			if (cup.CheckBoxHit(beef[i].GetPosX(), beef[i].GetPosY())) {
				beef[i].isScoop = true;	//	範囲内の牛肉をTRUE
				beef_in++;
			}
		}
		//	牛肉の量が２０より多いならおたまからはみ出るくらい広げる
		for (int i = 0; i < BEEF_NUM; i++) {
			if (beef[i].isScoop) {
				if (beef_in >= 20) {
					beef[i].scoop_offsetX = GetRand(170) - 85;
					beef[i].scoop_offsetY = GetRand(120) - 60;
				}
				//	２０未満ならおたまからはみ出ないように
				else {
					beef[i].scoop_offsetX = GetRand(130) - 65;
					beef[i].scoop_offsetY = GetRand(100) - 50;
				}
			
			}
		}
		//	玉ねぎも牛肉と同様
		int onion_in = 0;
		for (int i = 0; i < ONION_NUM; i++) {
			if (cup.CheckBoxHit(onion[i].GetPosX(), onion[i].GetPosY())) {
				onion[i].isScoop = true;
				onion_in++;
			}
		}
		for (int i = 0; i < ONION_NUM; i++) {
			if (onion[i].isScoop)
			{
				if (onion_in >= 5) {
					onion[i].scoop_offsetX = GetRand(140) - 70;
					onion[i].scoop_offsetY = GetRand(100) - 50;
				}
				else {
					onion[i].scoop_offsetX = GetRand(100) - 50;
					onion[i].scoop_offsetY = GetRand(80) - 40;
				}
			}
			
		}
	}

	//	３：すくわれている具材はカップ中心に配置
	int cx = cup.GetCenterX();
	int cy = cup.GetCenterY();
	for (int i = 0; i < BEEF_NUM; i++) {
		if (beef[i].isScoop) {
			beef[i].SetPos(cx + beef[i].scoop_offsetX,	//	offsetをつけないと全部中央にかたまってしまう
				cy + beef[i].scoop_offsetY);
		}
	}
	for (int i = 0; i < ONION_NUM; i++) {
		if (onion[i].isScoop) {
			onion[i].SetPos(cx + onion[i].scoop_offsetX,
				cy + onion[i].scoop_offsetY);
		}
	}

	//----------------------------------------------------------
	//	やること２　ごはんに引き上げた具材を盛り付ける

	//	プレイモードの時はごはんにのっている具材を数える
	if (phase == PLAY) {
		beef_in_rice = 0;
		onion_in_rice = 0;
		for (int i = 0; i < BEEF_NUM; i++) {
			if (!beef[i].isScoop &&
				rice.IsInside(beef[i].GetPosX(), beef[i].GetPosY()))
			{
				beef_in_rice++;
			}
		}
		for (int i = 0; i < ONION_NUM; i++) {
			if (!onion[i].isScoop &&
				rice.IsInside(onion[i].GetPosX(), onion[i].GetPosY()))
			{
				onion_in_rice++;
			}
			
		}
	}

	//----------------------------------------------------------
	//	やること３　エンター → 提供の流れ
	switch (phase)
	{
	case PLAY:
		end_alpha = 0;

		if (key.CheckPressKey(KEY_INPUT_RETURN)) {
			//	この瞬間にごはんの上にある具材を確定
			for (int i = 0; i < BEEF_NUM; i++)
				beef[i].in_rice = !beef[i].isScoop &&
				rice.IsInside(beef[i].GetPosX(), beef[i].GetPosY());
			for (int i = 0; i < ONION_NUM; i++)
				onion[i].in_rice = !onion[i].isScoop &&
				rice.IsInside(onion[i].GetPosX(), onion[i].GetPosY());

			//	お玉を初期位置へ
			cup.SetPos(900, 500);
			handle.SetPos(1070, 630);

			for (int i = 0; i < BEEF_NUM; i++)  beef[i].isScoop = false;
			for (int i = 0; i < ONION_NUM; i++) onion[i].isScoop = false;


			phase =SLIDE_IN;
			phase_timer = 0;
			dark_on = true;

		}
		break;

	case SLIDE_IN:
		MoveServing(8, 0);
		if (++phase_timer >= 60) {
			phase = SELECT;
			yes_no = 0;
		}
		break;

	case SELECT:
		if (key.CheckPressKey(KEY_INPUT_LEFT))  yes_no = 0;
		if (key.CheckPressKey(KEY_INPUT_RIGHT)) yes_no = 1;
		if (key.CheckPressKey(KEY_INPUT_RETURN)) {
			phase = (yes_no == 0) ? SERVE : RETURN;
			phase_timer = 0;
			dark_on = false;
		}
		break;

		//	提供
	case SERVE:
		MoveServing(0, -8);
		//	ー２００までいったら止める
		if (rice.GetPosY() < -200) {
			Judge();
			//　そこで止める
			MoveServing(0, 0);
			phase = COMMENT;
		}
		break;
		//	戻す
	case RETURN:
		MoveServing(-8, 0);
		if (++phase_timer >= 60) {
			for (int i = 0; i < BEEF_NUM; i++)  beef[i].in_rice = false;
			for (int i = 0; i < ONION_NUM; i++) onion[i].in_rice = false;
			phase = PLAY;
		}
		break;
		//	客コメント　提供後
	case COMMENT:
		//	客コメント表示時間
		phase_timer++;
		//	2秒超えたらリセット
		if (phase_timer >= 120) {
			serve_count++;				//	提供数１あげる
			order_size = GetRand(1);	//	次の注文をランダムに

			rice.SetPos(20, 490);
			for (int i = 0; i < BEEF_NUM; i++) {

				beef[i].GetRandomF(370, 870, 190, 670);	//	ランダム設定
				beef[i].SetRandomAngle();	//	角度をランダムに
				beef[i].in_rice = false;

			}
			for (int t = 0; t < ONION_NUM; t++) {

				onion[t].GetRandomF(370, 810, 170, 650);
				onion[t].SetRandomAngle();
				onion[t].in_rice = false;

			}
			//	終了するか次にいくか
			if (serve_count >= SERVE_MAX) {
				phase = END;			//	5回で終了
				phase_timer = 0;
			}
			else {
				phase = PLAY;
			}
		}
		break;

	case END:
		end_alpha+=4;
		if (end_alpha > 200) end_alpha = 200;

		// R キーでタイトルへ
		if (end_alpha == 200 && key.CheckPressKey(KEY_INPUT_R)) {
			manager->ChangeState(std::make_unique<TitleState>(manager));
		}

		break;

	}

	if (key.CheckReleaseKey(KEY_INPUT_M)) {
		pushMKey = !pushMKey;
	}

	//	マニュアルのスライド（目標に向かって動かす）
	//	ここはＡＩに聞きました
	float target_y = 0;
	if (pushMKey) {
		target_y = MANUAL_SHOW_Y;  
	}
	else {
		target_y = MANUAL_HIDE_Y;  
	}
	float speed = 30.0f;
	if (manual_y > target_y) {
		manual_y -= speed;
		if (manual_y < target_y) manual_y = target_y;
	}
	else if (manual_y < target_y) {
		manual_y += speed;
		if (manual_y > target_y) manual_y = target_y;
	}
	manual.SetPos(MANUAL_SHOW_X, (int)manual_y);

	//	湯気の描画
	//	範囲for文を使うことで添え字やイテレータを書かなくて済み、範囲外アクセスのミスも防げる
	for (auto& s : steam)
	{
		s.Update();
	}

}

//	描画処理
void StartGameState::Draw()
{
	bg.Draw();			//	背景
	pot.Draw();			//	鍋
	rice.Draw();		//	ごはん
	window.Draw();		//	窓口
	customer.Draw();	//	客
	SetFontSize(20);
	DrawString(15, 20, "店長", GetColor(255, 255, 255));

	//ｍキーボタン
	mButton.Draw();

	if (phase == COMMENT) {
		comment.Draw();
		SetFontSize(30);
		DrawString(200, 60, comment_text.c_str(), GetColor(0, 0, 0));
	}
	else if (start_timer < START_MSG_TIME) {
		comment.Draw();
		SetFontSize(25);
		DrawString(200, 30, "マニュアル通り", GetColor(255, 0, 0));
		DrawString(200, 70, "どんどん盛って", GetColor(0, 0, 0));
	}

	explain.Draw();		//	操作説明ボード

	//	オーダー並/大
	if (order_size == 0){
		order_normal.Draw();
	}
	else { 
		order_large.Draw();
	}
	
	//	左のときのカップ
	if (cup.IsLeft()) {
		cup.Draw();
	}
	//	交互に牛肉と玉ねぎを描画
	int bi = 0;
	int oi = 0;
	while (bi < BEEF_NUM || oi < ONION_NUM)
	{
		if (bi < BEEF_NUM && !beef[bi].isScoop)
		{
			beef[bi].Draw();
		}
		bi++;

		if (oi < ONION_NUM && !onion[oi].isScoop)
		{
			onion[oi].Draw();
		}
		oi++;
	}
	
	//	たれ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);	
	soup.Draw();		
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	

	//	湯気の描画
	for (const auto& s : steam) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
		s.Draw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//	右のときのカップ
	if (!cup.IsLeft()) {
		cup.Draw();

		//	交互に具材を描画
		int bi2 = 0;
		int oi2 = 0;
		while (bi2 < BEEF_NUM || oi2 < ONION_NUM)
		{
			if (bi2 < BEEF_NUM && beef[bi2].isScoop)
			{
				beef[bi2].Draw();
			}
			bi2++;

			if (oi2 < ONION_NUM && onion[oi2].isScoop)
			{
				onion[oi2].Draw();
			}
			oi2++;
		}

	}
	handle.Draw();		//	お玉の柄

	//	ごはんの上の具材の数
	SetFontSize(30);
	countBoard.Draw();	//	具材を数えるボード
	DrawFormatString(165, 415, GetColor(10, 10, 10), ": %d", beef_in_rice);
	DrawFormatString(200, 455, GetColor(10, 10, 10), ": %d", onion_in_rice);
	//	提供数
	SetFontSize(40);
	DrawFormatString(150, 360, GetColor(255, 255, 255), ": %d/5", serve_count);

	//	暗転(徐々に暗く/明るく)
	if (dark_on)
	{
		dark_alpha += 3;
		if (dark_alpha > 180) dark_alpha = 180;
	}
	else
	{
		dark_alpha -= 2;
		if (dark_alpha < 0)   dark_alpha = 0;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, dark_alpha);
	DrawFillBox(0, 0, 1280, 720, GetColor(0, 0, 0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//	提供中は、ごはんと具材を暗転の上に描き直す
	if (phase != PLAY) {
		rice.Draw();

		//	通常描画と同じく、牛肉と玉ねぎを交互に描く
		int bi3 = 0;
		int oi3 = 0;
		while (bi3 < BEEF_NUM || oi3 < ONION_NUM)
		{
			if (bi3 < BEEF_NUM && beef[bi3].in_rice)
			{
				beef[bi3].Draw();
			}
			bi3++;

			if (oi3 < ONION_NUM && onion[oi3].in_rice)
			{
				onion[oi3].Draw();
			}
			oi3++;
		}
	}

	//	はい / いいえ
	if (phase == SELECT) {
		SetFontSize(40);
		int yes_col = (yes_no == 0) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
		int no_col = (yes_no == 1) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
		DrawString(450, 230, "提供する？", GetColor(255, 255, 255));
		DrawString(450, 300, "はい", yes_col);
		DrawString(600, 300, "いいえ", no_col);
	}


	//	提供５回終了時の暗転
	if (phase == END) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, end_alpha);
		DrawFillBox(0, 0, 1280, 720, GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//	最大まで上がったら結果を書く
		if (end_alpha == 200) {

			//	終了用の客とコメントの描画
			customer2.Draw();	
			comment2.Draw();

			//	各ポイントと5回の提供数から平均を求める
			float point_result = (float)customer_point_sum / SERVE_MAX;

			SetFontSize(60);
			DrawFormatString(400, 50, GetColor(0, 255, 255),"総評：%0.1f", point_result);
			SetFontSize(40);
			//	Rキーでタイトルへ戻る画像
			rButton.Draw();
		
			//	文字列用変数
			const char* review1;	//	1行目
			const char* review2;	//	2行目（なければ空）

			if (point_result < 1.5f) {
				review1 = "はい、クビ";
				review2 = "";
			}
			else if (point_result < 2.5f) {
				review1 = "もっと努力";
				review2 = "しよっかｗ";
			}
			else if (point_result < 3.5f) {
				review1 = "その調子！";
				review2 = "";
			}
			else if (point_result < 4.5f) {
				review1 = "惜しいよ！";
				review2 = "いい感じ";
			}
			else {
				review1 = "今日から君は";
				review2 = "店長だ。";
			}
			//	吹き出しから調整して値調整しやすく
			DrawString(comment_end_x+90, comment_end_y+170, review1, GetColor(0, 0, 0));
			DrawString(comment_end_x+90, comment_end_y+230, review2, GetColor(0, 0, 0));	//	行間は文字サイズ+10くらい
		}
	}

	if (manual_y < MANUAL_HIDE_Y) {
		manual.Draw();
	}
}

//  判定  ここはAIを使って行数短縮させた
void StartGameState::Judge()
{
	//	規定量（並と大で範囲だけ違う）
	int beef_min, beef_max, onion_min, onion_max;
	if (order_size == 0) {	//	並
		beef_min = 12; beef_max = 18;
		onion_min = 4;  onion_max = 8;
	}
	else {					//	大
		beef_min = 19; beef_max = 26;
		onion_min = 9;  onion_max = 15;
	}

	//	-1:少ない  0:ちょうど  1:多い
	int b = (beef_in_rice < beef_min) ? -1 : (beef_in_rice > beef_max) ? 1 : 0;
	int o = (onion_in_rice < onion_min) ? -1 : (onion_in_rice > onion_max) ? 1 : 0;

	if (b == 0 && o == 0) { comment_text = "ありがとう！！";   customer_point = 5; }
	else if (b == -1 && o == -1) { comment_text = "やる気あるの？";   customer_point = 1; }
	else if (b == 1 && o == 1) { comment_text = "両方多すぎよ";     customer_point = 3; }
	else if (b == -1 && o == 0) { comment_text = "肉少ないぞ";       customer_point = 3; }
	else if (b == 0 && o == -1) { comment_text = "たまねぎ少な、";   customer_point = 3; }
	else if (b == 0 && o == 1) { comment_text = "たまねぎおおい";   customer_point = 4; }
	else if (b == 1 && o == 0) { comment_text = "肉おおいなあ？";   customer_point = 4; }
	else if (b == -1 && o == 1) { comment_text = "バランスが悪いな"; customer_point = 2; }
	else { comment_text = "玉ねぎばっかりだな"; customer_point = 2; }	//	b==1 && o==-1

	customer_point_sum += customer_point;
}








