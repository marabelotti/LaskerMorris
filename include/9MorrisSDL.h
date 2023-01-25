/**
 \file   9MorrisSDL.h
 \brief  class NineMorrisSDL : public Game_9Morris
 \author Mara Belotti
 \version   1.0
 \date      25 January 2023
*/

#ifndef NINEMORRISSDL_H
#define NINEMORRISSDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "Game_9Morris.h"

const int NUMBER_OF_POSITIONS = 24;

//! A class to visualize play Nine Men's Morris using SDL.
/*!
    This is a class that allows to render the game using SDL. SOme of the code has been copied from the turorials
    of Lazy Foo.
*/
class NineMorrisSDL: public Game_9Morris{
    public:
        enum PositionSprite
        {
            POSITION_SPRITE_MOUSE_OUT = 0,
            POSITION_SPRITE_MOUSE_OVER_MOTION = 1,
            POSITION_SPRITE_MOUSE_DOWN = 2,
            POSITION_SPRITE_MOUSE_UP = 3,
            POSITION_SPRITE_TOTAL = 4
        };

        class PositionSDL{
            friend class NineMorrisSDL;
            public:
                PositionSDL(int x_coord,int y_coord);
                void handleEvent( SDL_Event* e );
            
            private:
                PositionSprite mSprite;
                SDL_Point mPosition;
                const int pos_width = 50;
                const int pos_height = 50;
        };

        NineMorrisSDL(int screen_dim=720);

        ~NineMorrisSDL();

        void visual_game();
    
    private:
        int screen_width;
        int screen_height;
        std::string message;

        SDL_Window* gWindow = NULL;
        SDL_Renderer* gRenderer = NULL;
        SDL_Texture* gTexture = NULL;
        TTF_Font* font=NULL;
        std::vector<PositionSDL> posSDL; 

        bool init();
        void init_positions();
        bool background();
        void close();
        void render_position(int i);
        void render_information(std::string message);
        void get_message(bool turn);

        SDL_Texture* background_surface_to_texture(std::string path);

        virtual bool make_a_move_placing(bool turn);
        virtual bool make_a_move_moving(bool turn);
        virtual bool make_a_move_flying(bool turn);
        virtual void check_if_mills_and_remove(bool turn);
};

#endif //NINEMORRISSDL_H