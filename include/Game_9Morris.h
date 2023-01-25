/**
 \file   Game_9Morris.h
 \brief  The class Game_9Morris.
 \author Mara Belotti
 \version   1.0
 \date      16 January 2023
*/
#ifndef GAME_9MORRIS_H
#define GAME_9MORRIS_H

#include <vector>
#include <string>
#include <iostream>
#include "Game.h"
#include "Board_9Morris.h"
#include "alpha_beta.h"

//! The class Game_9Morris
/* This class implements the specifics of the Board Game_9Morris, which now knows how to make a move for the game 
    in the function play of Game.
*/
class Game_9Morris: public Game, public Board9Morris {
    public:
        //!A constructor
        /* Constructor for the class Game_9Morris, similar to the constructor of Game;
        */
        Game_9Morris(PlayerType type_of_white,PlayerType type_of_black,std::string name_of_white="White player", std::string name_of_black="Black Player");

        //!A game_over functions
        /* This function tells us wheter the game has ended or not.
        */                
        virtual Board::GameEnd game_over();
    protected:
        bool make_a_move(bool turn);
        virtual bool make_a_move_placing(bool turn);
        virtual bool make_a_move_moving(bool turn);
        virtual bool make_a_move_flying(bool turn);

        bool values_are_okay(bool turn,int pos_fin,int pos_int = -1);
        virtual void check_if_mills_and_remove(bool turn);

};

#endif //GAME_9MORRIS_H