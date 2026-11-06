#pragma once
#include "Dxlib.h"
/// <summary>
/// 基本の処理　よく使う関数もここにいれるとベスト
/// </summary>
class Base
{
protected:
	int pos_x = 0; int pos_y = 0;	
	int move_x = 0; int move_y = 0;
	float angle = 0.0f;	//	角度　

public:

	int GetPosX() const { return pos_x; }
	int GetPosY() const { return pos_y; }
	int GetMoveX() const { return move_x; }
	int GetMoveY() const { return move_y; }

	void SetPos(int x, int y) { pos_x = x; pos_y = y; }
	void Move(int mvx, int mvy) { pos_x += mvx; pos_y += mvy; }
	
	//	ランダムに指定範囲に置く　
	//	範囲は一番左が基準、そして右が最後　範囲は右から左までランダム
	void GetRandomF(int left, int right, int up, int down) 
	{
		int x = left + rand() % (right - left);
		int y = up + rand() % (down - up);
		SetPos(x, y);
	}

	//	角度をランダムにする
	void SetRandomAngle()
	{
		float deg = rand() % 360;
		angle = deg * 3.14f / 180.0f;	//	ラジアンに変換
	}

	

};