#pragma once
#ifndef GHOST_H_
#define GHOST_H_

#include "BaseFunction.h"
#include "BaseObject.h"
#include <cstdlib>
#define GHOST_SPEED 16;

class Ghost : public BaseObject
{
public:
	Ghost(int x, int y);
	~Ghost();

	enum WalkType
	{
		WALK_RIGHT = 0,
		WALK_LEFT = 1,
		WALK_UP = 2,
		WALK_DOWN = 3,
	};
	int x_pos_;
	int y_pos_;
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void set_clips();
	void SimpleAI(SDL_Event events, SDL_Renderer* screen);
	void DoGhost(Map& map_data);
	void CheckToMap(Map& map_data);
	bool checkWall(Map map_data);
private:
	float x_val_;
	float y_val_;



	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[4];
	Input input_type_;
	int frame_;
	int status_;

};
#endif // !GHOST_H_
