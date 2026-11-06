#pragma once
#include "Graph.h"
 
/// <summary>
/// 湯気用クラス
/// </summary>
class Steam:public GraphDraw
{
private:
	
	float anim_frame = 0;	//	アニメーションフレーム
	int anim_step = 0;	//	アニメーションステップ

public:

	Steam() = default;
	Steam(std::string arg_file_name, int x, int y)
	{
		Load(arg_file_name);
		SetPos(x, y);
	}
	void Update();
	void Draw()const override;
};