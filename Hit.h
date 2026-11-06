#pragma once
#include "Dxlib.h"

//	あたり判定系を書いているクラス
class CheckHit
{
protected:

	int left = 0;	int right = 0;	//	左右
	int top = 0;	int bottom = 0;	//	上下	

	//	判定微調整用
	int margin_left = 0; int margin_right = 0;
	int margin_top = 0;  int margin_bottom = 0;

public:

	//	微調整設定関数
	void SetHitMargin(int l, int r, int t, int b)
	{
		margin_left = l;
		margin_right = r;
		margin_top = t;
		margin_bottom = b;
	}

	//	四角あたり判定　微調整つき
	bool CheckBoxHit(int x, int y) const
	{
		return x >= left - margin_left
			&& x <= right + margin_right
			&& y >= top - margin_top
			&& y <= bottom + margin_bottom;
	}

	//	円のあたり判定
	bool CheckCircleHit(int x, int y, 
		int center_x, int center_y, int radius)
	{
		int dx = x - center_x;
		int dy = y - center_y;
		if (dx * dx + dy * dy <= radius * radius){
			return true;
		}
		else{
			return false;
		}
	}



};