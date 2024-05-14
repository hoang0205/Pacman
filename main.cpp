#include "BaseFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "Character.h"
#include "ImpTime.h"
#include "Ghost.h"
#include <vector>
#include <ctime>
#include <iostream>
#include "Text.h"

BaseObject g_background;
TTF_Font* font = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
    {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    if (TTF_Init() == -1)
    {

        success = false;
    }
    font = TTF_OpenFont("font//dlxfont_.ttf", 12);
    if (font == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {   
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    g_window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) success = false;
        }

    }
    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//bkground.png",g_screen);
    if (ret == false) return false;

    return true;
}

void close()
{
    g_background.Free();
    
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    ImpTimer fps_timer;

    if (InitData() == false) return -1;
    if (LoadBackground() == false) return -1;

    GameMap game_map;
    game_map.LoadMap((char*)"map/map01.dat");
    game_map.LoadTiles(g_screen);
    Map map_data = game_map.getMap();
    MainObject p_player;
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clips();
    srand(time(0));
    std::vector<Ghost> g_ghost;
    int x = 0, y = 0;
    while(g_ghost.size() < NUMBER_OF_GHOST)
    {
         x = 1 + (int)(rand() % (1344 - 1 + 1));
         y = 1 + (int)(rand() % (448 - 1 + 1));
         //std::cout << x << " " << y << '\n';
         Ghost ghost(x, y);
         if(ghost.checkWall(map_data) == true) 
             g_ghost.push_back(ghost);
    }
    for (int i = 0; i < NUMBER_OF_GHOST; i++)
    {
        g_ghost[i].LoadImg("img//ghost_right.png", g_screen);
        g_ghost[i].set_clips();
    }

    TextObject time_game;
    time_game.SetColor(TextObject::RED_);

    Mix_Chunk* newSound = Mix_LoadWAV("sound/pacman.wav");

    bool is_quit = false;
    while (!is_quit)
    {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            p_player.HandleInputAction(g_event, g_screen);
            
        }
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);
        
        g_background.Render(g_screen, NULL);
        for (int i = 0; i < NUMBER_OF_GHOST; i++) g_ghost[i].SimpleAI(g_event, g_screen);
        p_player.DoPlayer(map_data);
        for (int i = 0; i < NUMBER_OF_GHOST; i++) g_ghost[i].DoGhost(map_data);
    
        game_map.update(map_data);
        game_map.DrawMap(g_screen);
        for (int i = 0; i < NUMBER_OF_GHOST; i++) p_player.CheckGhost(g_ghost[i]);
        p_player.Show(g_screen);
        for (int i = 0; i < NUMBER_OF_GHOST; i++) g_ghost[i].Show(g_screen);
        p_player.endGame();
        if (p_player.alive == 0) break;
        SDL_RenderPresent(g_screen);

        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        std::string str_val = std::to_string(time_val);
        str_time += str_val;


        time_game.SetText(str_time);
        time_game.LoadFromRenderText(font, g_screen);
        time_game.renderText(g_screen, SCREEN_WIDTH - 200, 15);
        
        Mix_PlayChannel(-1, newSound, -1);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND;

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >= 0)
            SDL_Delay(delay_time);
        }
        
    }
    close();

    return 0;
}