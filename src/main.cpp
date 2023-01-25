#include <memory>
#include <chrono>
#include <stdlib.h>
#include <omp.h>
#include "9MorrisSDL.h"

//!Main file
/* Run with 
    make
    ./NineMenMorris
*/

int main(){

    setenv("OMP_NUM_THREADS", "8", true);

    std::cout<<"°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n";
    std::cout<<"Second test: PLAY A GAME USING SDL SUPPORT\n";
    std::cout<<"Here you can try to play a game against the AI using SDL support\n";
    std::cout<<"°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n";
    {
        NineMorrisSDL game;
        game.visual_game();
    }

}