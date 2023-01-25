/**
 \file   Game.h
 \brief  The class Game can be used together with an object of type Board to play a game.
 \author Mara Belotti
 \version   1.0
 \date      16 January 2023
*/

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <iostream>
#include "Board.h"

//! The class Game for two player games with alternating moves
/*! This constructs a Game with some important features. It is important for children classes to be children classes of Board
    and to have a right notion of game_over.
*/
class Game {
    public:
        //! An enumeration
        /*! This constructs an enumeration type that allows to distinguish a Human player from an AI  
        */
        enum PlayerType : int {
            Human=1, /**< Human player */
            AI=-1 /**< AI player */
        };

         //! A constructor
        /*! This constructs a game. One needs to specify the types of the players and a name for them;   
        */
        Game(PlayerType type_of_white,PlayerType type_of_black,std::string name_of_white, std::string name_of_black);
        void play();

    protected:
        class Player{
            public:
                Board::PlayerColor player_color;
                PlayerType player_type; 
                std::string player_name;
        };

        //The first is white, the other black
        std::vector<Player> players=std::vector<Player>(2);

        virtual bool make_a_move(bool turn)=0;
        virtual Board::GameEnd game_over()=0;
};

#endif //GAME_H