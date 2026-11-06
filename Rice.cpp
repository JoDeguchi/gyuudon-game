#include "Rice.h"

void Rice::Draw()const
{
	if (graph_hnd == -1) return;
	//	ずれ補正付き
	DrawGraph(pos_x , pos_y , graph_hnd, true);
}