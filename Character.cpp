﻿#include "Character.h"
#include <Windows.h>
#include <sstream>
#include <string>

MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = -1;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
}

MainObject::~MainObject()
{
	Free();
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret == true)
	{
		width_frame_ = rect_.w / 4;
		height_frame_ = rect_.h;

	}
	return ret;

}

void MainObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		frame_clip_[1].x = width_frame_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = width_frame_;
		frame_clip_[1].h = height_frame_;

		frame_clip_[2].x = 2*width_frame_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = width_frame_;
		frame_clip_[2].h = height_frame_;

		frame_clip_[3].x = 3*width_frame_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = width_frame_;
		frame_clip_[3].h = height_frame_;
	}
}

void MainObject::Show(SDL_Renderer* des)
{
	switch (status_)
	{
	    case WALK_LEFT:
	    {
			LoadImg("img//player_left.png", des);
	    }
		break;
		case WALK_RIGHT:
		{
			LoadImg("img//player_right.png", des);
		}
		break;
		case WALK_UP:
		{
			LoadImg("img//player_up.png", des);
		}
		break;
		case WALK_DOWN:
		{
			LoadImg("img//player_down.png", des);
		}
		break;
	}
	if (input_type_.left_ == 1 || input_type_.right_ == 1 || input_type_.up_ == 1 || input_type_.down_ == 1)
	{
		frame_++;
	}
	else
	{
		frame_ = 0;
	}
	if (frame_ >= 4)
	{
		frame_ = 0;
	}
	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		 case SDLK_RIGHT:
		 {
			 status_ = WALK_RIGHT;
			 input_type_.right_ = 1;
			 input_type_.left_ = 0;
			 input_type_.up_ = 0;
			 input_type_.down_ = 0;
		 }
		 break;
		 case SDLK_LEFT:
		 {
			 status_ = WALK_LEFT;
			 input_type_.right_ = 0;
			 input_type_.left_ = 1;
			 input_type_.up_ = 0;
			 input_type_.down_ = 0;
		 }
		 break;
		 case SDLK_UP:
		 {
			 status_ = WALK_UP;
			 input_type_.right_ = 0;
			 input_type_.left_ = 0;
			 input_type_.up_ = 1;
			 input_type_.down_ = 0;
		 }
		 break;
		 case SDLK_DOWN:
		 {
			 status_ = WALK_DOWN;
			 input_type_.right_ = 0;
			 input_type_.left_ = 0;
			 input_type_.up_ = 0;
			 input_type_.down_ = 1;
		 }
		 break;
		 default:
			 break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			input_type_.right_ = 0;
			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			input_type_.right_ = 0;
			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_UP:
		{
			input_type_.right_ = 0;
			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.right_ = 0;
			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		break;
		default:
			break;
		}
	}
}

void MainObject::DoPlayer(Map& map_data)
{
	x_val_ = 0;
	y_val_ = 0;
	if (input_type_.left_ == 1)
	{
		x_val_ -= PLAYER_SPEED;
	}
	else if (input_type_.right_ == 1)
	{
		x_val_ += PLAYER_SPEED;
	}
	else if (input_type_.up_ == 1)
	{
		y_val_ -= PLAYER_SPEED;
	}
	else if(input_type_.down_ == 1)
	{
		y_val_ += PLAYER_SPEED;
	}
	CheckToMap(map_data);
}

void MainObject::CheckToMap(Map& map_data)
{
	x_pos_ += x_val_;
	y_pos_ += y_val_;
	if ((x_pos_ < 0) || (x_pos_ + TILE_SIZE > SCREEN_WIDTH))
	{
		x_pos_ -= x_val_;
	}
	if ((y_pos_ < 0) || (y_pos_ + TILE_SIZE > SCREEN_HEIGHT))
	{
		y_pos_ -= y_val_;
	}
	{
		int x1, x2, y1, y2;
		x1 = int(x_pos_);
		y1 = int(y_pos_);
		if (int(x_pos_ ) % TILE_SIZE == 0)
		{
			x1 = x1 / TILE_SIZE;
			x2 = x1;
		}
		else
		{
			x1 = x1 / TILE_SIZE ;
			x2 = x1 + 1;
		}
		if (int(y_pos_ ) % TILE_SIZE == 0)
		{
			y1 = y1 / TILE_SIZE ;
			y2 = y1;
		}
		else
		{
			y1 = y1 / TILE_SIZE ;
			y2 = y1 + 1;
		}
		if (map_data.tile[y1][x1] == 1)
		{
			map_data.tile[y1][x1] = 0;
			Point++;
		}
		if (map_data.tile[y1][x2] == 1)
		{
			map_data.tile[y1][x2] = 0;
			Point++;
		}
		if(map_data.tile[y2][x1] == 1)
		{
			map_data.tile[y2][x1] = 0;
			Point++;
		}
		if (map_data.tile[y2][x2] == 1)
		{
			map_data.tile[y2][x2] = 0;
			Point++;	
		}
		
		if (map_data.tile[y1][x1] > 0 || map_data.tile[y1][x2] > 0 || map_data.tile[y2][x1] > 0 || map_data.tile[y2][x2] > 0)
		{
			x_pos_ -= x_val_;
			y_pos_ -= y_val_;
		}
	}

}
void MainObject::CheckGhost(Ghost ghost)
{
	bool game_over = false;
	if (x_pos_ < ghost.x_pos_ && ghost.x_pos_ < x_pos_ + TILE_SIZE && y_pos_ < ghost.y_pos_ && ghost.y_pos_ < y_pos_ + TILE_SIZE)
	{
		game_over = true;
	}
	if (x_pos_ > ghost.x_pos_ && ghost.x_pos_ + TILE_SIZE > x_pos_ && y_pos_ < ghost.y_pos_ && ghost.y_pos_ < y_pos_ + TILE_SIZE)
	{
		game_over = true;
	}
	if (x_pos_ < ghost.x_pos_ && ghost.x_pos_ < x_pos_ + TILE_SIZE && y_pos_ < ghost.y_pos_ + TILE_SIZE && ghost.y_pos_ < y_pos_)
	{
		game_over = true;
	}
	if (x_pos_ > ghost.x_pos_ && ghost.x_pos_ + TILE_SIZE > x_pos_ && y_pos_ < ghost.y_pos_ + TILE_SIZE && ghost.y_pos_ < y_pos_)
	{
		game_over = true;
	}
	if (game_over == true)
	{
		int Score = Point * 100;
		std::wstring message = L"Game Over\nYour Score is: " + std::to_wstring(Score);

		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
		{
			MessageBox(NULL, message.c_str(), L"Notification", MB_OK | MB_ICONQUESTION);
			alive = false;
			SDL_Quit();
			//return 0;
		}
	}
}
void MainObject::endGame()
{
	if (Point == MAX_POINT)
	{
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
		{
			MessageBox(NULL, L"You won :))", L"Congratulation", MB_OK | MB_ICONQUESTION);
			alive = false;
			SDL_Quit();
			//return 0;
		}
	}
}