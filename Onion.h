#pragma once
#include  "Graph.h"

class Onion :public GraphDraw
{
private:
	
public:
	bool isScoop = false;	//	お玉にすくわれているか

	int scoop_offsetX = 0;	//	カップ中心からのずれ
	int scoop_offsetY = 0;

	bool in_rice = false;

	void Update();
	void Draw()const override;
};