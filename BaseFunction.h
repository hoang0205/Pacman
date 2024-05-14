#pragma once
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

const int SCREEN_WIDTH = 1344;
const int SCREEN_HEIGHT = 448;
const int BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 255;

const int FRAME_PER_SECOND = 25;

const int NUMBER_OF_GHOST = 5;
const int MAX_POINT = 403;

#define TILE_SIZE 32
#define MAX_MAP_X 42
#define MAX_MAP_Y 14

struct Map
{
	int start_x_;
	int start_y_;

	int max_x_;
	int max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name_;
};
struct Input
{
	int left_;
	int right_;
	int up_;
	int down_;
};

#endif // !COMMON_FUNCTION_H_