#include "BaseFunction.h"
#include "Text.h"
SDL_Surface* SDL_Common::loadSurface(std::string path)
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL)
        {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}
int SDL_Common::ShowMenu(SDL_Surface* des, TTF_Font* font)
{
    g_menu = loadSurface("img//menu.png");
    if (g_menu == NULL)
    {
        return -1;
    }

    const int MenuItems = 2;
    SDL_Rect pos_arr[MenuItems];
    pos_arr[0].x = 200;
    pos_arr[0].y = 400;

    pos_arr[1].x = 200;
    pos_arr[1].y = 200;

    TextObject text_menu[MenuItems];

    text_menu[0].SetText("Play");
    text_menu[0].setColor(TextObject::WHITE_TEXT);
    text_menu[0].loadFromRenderedText(font, g_screen);
    text_menu[0].RenderText(g_screen, SCREEN_WIDTH - 200, 15);
    SDL_Event m_event;
    while (SDL_PollEvent(&m_event))
    {
        switch (m_event.key.keysym.sym)
        {
        case SDLK_SPACE: 
        {
            return 1;
        }
        default:
            break;
        }
    }
}