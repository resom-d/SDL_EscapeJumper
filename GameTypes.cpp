#include "GameTypes.h"

MatrixTile::MatrixTile()
{
	TileIndex = 0;
	Type = TileType::Background;
	BorderColor = 0;
	FillColor = 0;
	Visible = false;
}

MatrixTile::~MatrixTile()
{}
