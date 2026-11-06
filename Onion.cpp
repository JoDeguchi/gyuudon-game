#include "Onion.h"

void Onion::Update()
{
	
}

void Onion::Draw() const
{
	if (graph_hnd == -1) return;
	DrawRotaGraph(pos_x, pos_y, 1.0f, angle, graph_hnd, true);
}