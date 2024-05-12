#include "BaseFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "Character.h"
#include "ImpTime.h"
#include "Ghost.h"
#include <vector>
#include <ctime>
#include <iostream>
BaseObject g_background;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
    {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

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
        
        SDL_RenderPresent(g_screen);
        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND;

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >= 0)
            SDL_Delay(delay_time);
        }
        p_player.endGame();
        if (p_player.alive == false) break;
    }

    close();

    return 0;
}