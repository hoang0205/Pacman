#pragma once
#ifndef GAME_MAP_H
#define GAME_MAP_H
#define MAX_TILE 20

#include "BaseFunction.h"
#include "BaseObject.h"

class TileMap : public BaseObject
{
public:
	TileMap(){ ; }
	~TileMap() { ; }
};

class GameMap
{
public:	Map game_map;
	GameMap() { ; }
	~GameMap() { ; }
	
	void LoadMap(char* name);
	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* screen);
	Map getMap() const { return game_map; }
	void update(Map temp) { game_map = temp; }
private:

	TileMap tile_map[MAX_TILE];
};

#endif // !GAME_MAP_H_
