/**
 \file   Board.h
 \brief  class Board
 \author Mara Belotti
 \version   1.0
 \date      16 January 2023
*/

#ifndef BOARD_H
#define BOARD_H

#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <memory>


//! A board class.
/*!
    The class board implements a board for a variety of two player games with alternating moves 
    (ex Go, Chess, 9Men's Morris, Checkers...).
*/
class Board{
    public:
        /** 
        * An enum for player colors.
        * The color of a player can be either white or black.
        */

        enum PlayerColor : int {
            Black=1, /**< Black player */
            White=-1 /**< White player */
        };

        /** 
        * An enum for the type of pieces.
        * The color of a piece can be white, black or empty.
        */

        enum PieceType : int {
            Empty=0, /**< Empty position */
            Black_piece=1, /**< Black piece */
            White_piece=-1 /**< White piece */
        };

        /** 
        * An enum for the phase of the game.
        */

        enum GameEnd: int {
            NotGameOver=0,
            GameOver=1
        };
        
        /** 
        * This is a class called Position which keeps track of what is happening at a given position.
        * In particular it records the PieceType on the position and the positions which are adjacent 
        */
        class Position{
            friend class Board9Morris;
            friend class Board;
            friend class Game;
            friend class Game_9Morris;
            private:
                std::vector<int> adj_positions;
                PieceType piece;
            public:
                /**
                * This function returns the vector of indices of positions which are adjacent to Position.
                */
                std::vector<int> adjacent_positions() const;
                /**
                * This function returns the PieceType on the current Position
                */
                PieceType piece_on_position() const;
        };

        //! A constructor.
        /*!
            This is a constructor for the class Board. The Board is a graph and a file in the right format must be provided 
            in order to construct it. The initial value is put at 0.
        */
        Board();

        //! The default deconstructor.
        /*!
            We had to make this virtual not to have memory leaks: if a pointer points to a deriver class and we delete the object 
            pointed by it, the deconstructor will be the one of the derived class.
        */
        virtual ~Board()=default;

        //! A function to print
        /**
        * This is a function that prints the board into the terminal.
        */
        void print() const;

        //! Return number of pieces on board of a specified color
        /*!
            This function returns the number of pieces on the board of the color specified by the parameter of type PieceType
            \param  color of type PieceType, which specify the color of the pieces
            \return the number of the pieces of the specified color
        */
        
        int on_board(PlayerColor color) const;

        //! Return number of pieces still to play of a specified color
        /*!
            This function returns the number of pieces still to play of the color specified by the parameter of type PieceType
            \param  color of type PieceType, which specify the color of the pieces
            \return the number of the pieces of the specified color
        */

        int still_to_play(PlayerColor color) const;  

        //! Return number of pieces out of game for a specified color
        /*!
            This function returns the number of pieces out of play for the color specified by the parameter of type PieceType
            \param  color of type PieceType, which specify the color of the pieces
            \return the number of the pieces of the specified color out of game
        */ 

        int out_of_game(PlayerColor color) const;   

        //! Return the value of the game
        /*!
            Each board comes with a value which approximately encodes how the game is going.
            The bigger this value is, the closest is the Black color to win.
            The smallest this value is, the closest is the Black color to loose.
            The initial value is 0.
            \return the value of the board
        */
        float get_valuation() const;  
        
        //! Return the winner of the game of type PlayerColor played on Board.
        /*! If there is no winner yet, this function returns Empty.
            \return a PieceType value representing the winner.
        */
        PieceType give_winner() const;

        //! Return an empty vector
        /*! The output of this function is an empty vector for the class Board.
            \param player of type PlayerColor that is about to make a move 
            \return empty vector
        */
        virtual std::vector<std::unique_ptr<Board>> sons(PlayerColor player) const=0;

        //! Return wheter the game on the board has ended or not
        /*! The board also need this function for the alpha-beta algorithm.
        */
        GameEnd game_on_board_over();

    protected: 
        GameEnd game_end;
        float value;
        int num_pieces;
        PieceType winner=Empty;
        //The first two are black and the other two are white
        std::vector<int> pawn_state;
        std::vector<Position> positions;

        virtual void visual() const;

};

#endif //BOARD_H