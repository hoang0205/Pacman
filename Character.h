#pragma once
#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "BaseFunction.h"
#include "BaseObject.h"
#include "Ghost.h"
#define PLAYER_SPEED 8;
#define FOOD 1;

class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_RIGHT = 0,
	    WALK_LEFT = 1,
		WALK_UP = 2,
		WALK_DOWN = 3,
	};
	int Point = 0;
	int alive = 3;
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
	void set_clips();
	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);
	void CheckGhost(Ghost ghost);
	void endGame();
	void set_come_back_time(const int& cb_time) { come_back_time_ = cb_time; }
private:
	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[4];
	Input input_type_;
	int frame_;
	int status_;

	int come_back_time_;
};

#endif // !CHARACTER_H_
