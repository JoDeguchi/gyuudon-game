#include "Steam.h"

namespace
{
	constexpr int graphSizeW = 1000;	// 画像全体サイズ 幅
	constexpr int graphSizeH = 571;		// 画像全体サイズ 高さ
	constexpr int ANIM_COLS = 5;		// 横のコマ数
	constexpr int ANIM_ROWS = 3;		// 縦のコマ数
	constexpr int ANIM_FRAME_MAX = ANIM_COLS * ANIM_ROWS;	// 総コマ数
	
	constexpr int FRAME_W = graphSizeW / ANIM_COLS;	// 1コマの幅 
	constexpr int FRAME_H = graphSizeH / ANIM_ROWS;	// 1コマの高さ
	constexpr float animSpeed = 0.1f;	// アニメーションスピード

}

void Steam::Update()
{
	//	フレームを進める
	anim_frame += animSpeed;
	//	フレームが1以上になったら、次のコマへ
	if (anim_frame >= 1.0f)
	{
		anim_frame -= 1.0f;
		anim_step = (anim_step + 1) % ANIM_FRAME_MAX;	
	}
}

void Steam::Draw() const
{
	if (graph_hnd == -1) return;

	// コマ番号から、画像内の切り出し位置を計算
	int src_x = (anim_step % ANIM_COLS) * FRAME_W;	// 列
	int src_y = (anim_step / ANIM_COLS) * FRAME_H;	// 行

	DrawRectGraph(pos_x, pos_y, src_x, src_y, FRAME_W, FRAME_H, graph_hnd, TRUE);
}