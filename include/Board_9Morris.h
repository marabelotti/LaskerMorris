/**
 \file   Board_9Morris.h
 \brief  This implements a subclass of class Board for the game 9Morris.
 \author Mara Belotti
 \version   1.0
 \date      16 January 2023
*/

#ifndef BOARD_9MORRIS_H
#define BOARD_9MORRIS_H

#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include "Board.h"


//! The class Board9Morris implements a board for 9Morris.

class Board9Morris: public Board{
    public:
        /** 
        * An enum for the phase of the game.
        */
        enum GamePhase : int {
            Placing=0, /**< Placing phase: the player can only add new pieces until when all nine of them have been played */  
            Moving=1,  /**< Moving phase: the player can only move their pieces to adjacent positions*/ 
            Flying=2,  /**< Flying phase: if one player has only three pieces left, he can move his pawns to any empty positions*/ 
        };

        /** 
        * This is a subclass of Board which keeps track of what is happening at a given position.
        * In particular it records the PieceType on the position, the positions which are adjacent 
        * and the two ways to form a mill by putting a piece on that position
        */
        class Mills{
            friend Board9Morris;
            private:
                std::vector<int> structure=std::vector<int>(4);
        };
        
        //! A constructor.
        /*!
            This is a constructor for the class Board9Morris. It construct and empty board and 
            it initializes the game_phase to Placing. 
        */
        Board9Morris();

        //! Another constructor.
        /*!
        */
        Board9Morris(std::vector<int> v, Board9Morris::GamePhase phase, int wmov, int bmov,std::vector<int> pawn_state);

        virtual ~Board9Morris()=default;

        //! Return the phase of the game
        /*!
            Each board comes with a GamePhase. This function is giving this phase.
            \return the phase of the game of type GamePhase
        */
        GamePhase give_game_phase() const;

         //! Return the vector of all possible moves for the PlayerColor specified
        /*! The output of this function depends on the GamePhase of Board.
            \param player of type PlayerColor that is about to make a move 
            \return vector of Board object which register all possible moves for the player
        */
        virtual std::vector<std::unique_ptr<Board>> sons(PlayerColor player) const;

    protected: 
        GamePhase game_phase;
        int fresh_mills=0;
        int white_movibility=0;
        int black_movibility=0;
        std::vector<Mills> mills_structure=std::vector<Mills>(24);

        void check_game_phase();
        void evaluate_board();
        void check_mills(int i);
        int get_movibility(int i) const;

        virtual void visual() const;
        void drop(int i) const;

        void move_piece(int pos_in, int pos_fin, PlayerColor color);
        void remove_piece(int pos,PlayerColor color);
        void place_piece(int pos,PlayerColor color);

        std::vector<int> give_color_positions(PieceType color) const;
        std::vector<std::unique_ptr<Board>> sons_place(PlayerColor color) const;
        std::vector<std::unique_ptr<Board>> sons_move(PlayerColor color) const;
        std::vector<std::unique_ptr<Board>> sons_fly(PlayerColor color) const;


};

#endif //BOARD_9MORRIS_H