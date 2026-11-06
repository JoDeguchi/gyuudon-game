#include "Radle.h"


void Radle::Update()
{
	scoop_up = false;	//	ひきあげていない

	//	毎フレーム、移動量をリセット
	move_x = 0;
	move_y = 0;

	//	お玉の移動WASD
	if (CheckHitKey(KEY_INPUT_D)) { move_x += 6; }
	if (CheckHitKey(KEY_INPUT_A)) { move_x -= 6; }
	if (CheckHitKey(KEY_INPUT_W)) { move_y -= 6; }
	if (CheckHitKey(KEY_INPUT_S)) { move_y += 6; }

	Move(move_x, move_y);

	Read();	//	キーの読み込み　ないと左右キーが反応しない

	//	左キー：沈める
	if (CheckPressKey(KEY_INPUT_LEFT) && !sunk) {
		sunk = true;
		graph_hnd = hnd_left;
		offset_x = offset_left_x;
		offset_y = offset_left_y;
	}
	//	右キー：引き上げる
	else if (CheckPressKey(KEY_INPUT_RIGHT) && sunk) {
		sunk = false;
		scoop_up = true;	//	ここでひきあげるフラグON
		graph_hnd = hnd_right;
		offset_x = offset_right_x;
		offset_y = offset_right_y;
	}
}

void Radle::Draw() const
{
	if (graph_hnd == -1) return;
	//	ずれ補正付き
	DrawGraph(pos_x + offset_x, pos_y + offset_y, graph_hnd, true);
}

//	あたり判定追随範囲
void Radle::Range()
{
	left = pos_x;
	top = pos_y;

	right = left + 80;
	bottom = top + 80;
}
