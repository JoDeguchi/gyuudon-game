#pragma once
#include "Graph.h"
#include "KeyReader.h"
#include "Hit.h"

class Radle :public GraphDraw, KeyReader, public CheckHit
{
private:

	//	画像が２つあり切り替えるので
	//	新しい画像ハンドルを２個作る
	int hnd_right = -1;
	int hnd_left = -1;

	//	レードル切替の際少し誤差が生じるので、柄を少しずらす用の変数
	int offset_x = 0;	
	int offset_y = 0;

	//	右・左　それぞれのずれ補正
	int offset_right_x = 0;
	int offset_right_y = 0;
	int offset_left_x = 0;
	int offset_left_y = 0;

	bool sunk = false;		//	左キーで沈めた状態か
	bool scoop_up = false;	//	引き上げたか

public:

	//	コンストラクタで２種類画像を読み込む
	Radle() = default;
	Radle(std::string right_file, std::string left_file, int x, int y)
	{

		hnd_right = LoadGraph(right_file.c_str());
		hnd_left = LoadGraph(left_file.c_str());
		//	最初は右　誤差も右の補正
		graph_hnd = hnd_right;
		offset_x = offset_right_x;
		offset_y = offset_right_y;

		SetPos(x, y);
	}

	//	ずれ補正を設定　左右
	void OffsetRight(int x, int y) { offset_right_x = x; offset_right_y = y; }
	void OffsetLeft(int x, int y) { offset_left_x = x;  offset_left_y = y; }
	
	//	更新処理
	void Update();
	//	描画処理
	void Draw()const override;	

	//	現在左キー押している状態のお玉か
	bool IsLeft() const { return graph_hnd == hnd_left; }
	//	右キーを押して、沈めた状態から引き上げたか
	bool IsScoopUp() const { return scoop_up; }

	//	おたまのカップの範囲　おたまが動くので当たリ判定追随
	void Range();

	//	お玉カップの中心
	int GetCenterX() const
	{
		int w = 0, h = 0;
		GetGraphSize(graph_hnd, &w, &h);
		return pos_x + w / 2 ;
	}
	int GetCenterY() const
	{
		int w = 0, h = 0;
		GetGraphSize(graph_hnd, &w, &h);
		return pos_y + h / 2;
	}

};