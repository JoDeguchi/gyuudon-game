#pragma once
#include "Graph.h"
#include "Hit.h"

//	ごはん
class Rice:public GraphDraw,public CheckHit
{
private:
	//	ごはんの左上から、判定円の中心までの距離と半径
	int circle_dx = 120;
	int circle_dy = 105;
	int radius = 120;

public:
	//	コンストラクタ
	Rice() = default;
	Rice(std::string arg_file_path, int x, int y)
	{
		Load(arg_file_path);
		SetPos(x, y);
	}

	//	判定円の設定（画像に合わせて調整用）
	void SetCircle(int dx, int dy, int r) { circle_dx = dx; circle_dy = dy; radius = r; }

	//	指定した座標がごはん判定内にあるか
	bool IsInside(int x, int y)
	{
		return CheckCircleHit(x, y, pos_x + circle_dx, pos_y + circle_dy, radius);
	}

	void Draw() const override;

};