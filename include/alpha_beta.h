/**
 \file   alpha_beta.h
 \brief  The class alpha_beta implements an algorithm for choosing the next best move in a two player game.
 \author Mara Belotti
 \version   1.0
 \date      16 January 2023
*/

#ifndef ALPHA_BETA_H
#define ALPHA_BETA_H

#include <limits>
#include <iostream>
#include <cassert>
#include <tuple>
#include <omp.h>
#include "Board.h"

 //! The class alpha_beta implements an algorithm for choosing the next best move in a two player game.
/*!  This class implements the alpha_beta pruning algorithm for a two player game.
    However exploring the whole tree of possible moves would be computationally heavy 
    for complex game. That is why we specify a max_depth: the depth first search will only
    explore nodes up to max_depth.
    This class is templeted and T should be a derived class of Board.
    The depth at which the player running the algorithm achieve a win is kept in consideration using the
    variable cont.
*/
template <class T>
class alpha_beta{
private:
    int current_depth;
    float best_value;
    int moves_to_win;
    T & choosen_board;
    Board::PlayerColor color_of_player;
    int max_depth;

    std::tuple<float,int> run_alpha_beta(std::unique_ptr<Board>& board, Board::PlayerColor color,float alpha, float beta,int depth){

        float value;
        int cont;

        if (board->game_on_board_over()==Board::GameOver){ 
            assert(board->give_winner()!=Board::Empty);
            if(board->give_winner()==Board::White_piece){
                value=-std::numeric_limits<float>::max();
            }else{
                value=std::numeric_limits<float>::max();
            }
            cont=depth;
        }else if(depth==max_depth){
            value=board->get_valuation();
            cont=max_depth;
        }else if (color==Board::Black){
            value=-std::numeric_limits<float>::max();
            cont=max_depth;
            std::vector<std::unique_ptr<Board>> vec_ptr(board->sons(color));
            #pragma omp parallel for
                for(auto& el_ptr : vec_ptr){
                    float temp_value;
                    int temp_cont;
                    std::tie(temp_value, temp_cont)=run_alpha_beta(el_ptr,(Board::PlayerColor) -(int) color,alpha,beta,depth+1);
                    #pragma omp critical 
                    {
                        if(temp_value>=value){
                            cont=std::min(cont,temp_cont);
                        }
                        value=std::max(value,temp_value);
                        alpha= std::max(alpha, value);
                        if (alpha > beta){
                            //break;
                        }
                    }
                }
            vec_ptr.clear();
        }else {
            value=std::numeric_limits<float>::max();
            cont=max_depth;
            std::vector<std::unique_ptr<Board>> vec_ptr(board->sons(color));
            #pragma omp parallel for
                for(auto& el_ptr : vec_ptr){
                    float temp_value;
                    int temp_cont;
                    std::tie(temp_value, temp_cont)=run_alpha_beta(el_ptr,(Board::PlayerColor) -(int) color,alpha,beta,depth+1);
                    #pragma omp critical
                    {
                        if(temp_value<=value){
                            cont=std::min(cont,temp_cont);
                        }
                        value=std::min(value,temp_value);
                        beta= std::min(beta, value);
                        if (alpha > beta){
                            //break;
                        }
                    }
                }
            vec_ptr.clear();
        }

        if(depth==1){
            if(color_of_player==Board::Black and value>=best_value and moves_to_win>=cont){
                best_value=value;
                //Here we are just copying the value of the pointer into choosen_board
                choosen_board=*(dynamic_cast<T*>(board.get()));
                moves_to_win=cont;
            }   
            if(color_of_player==Board::White and value<=best_value and moves_to_win>=cont){
                best_value=value;
                //Here we are just copying the value of the pointer into choosen_board
                choosen_board=*(dynamic_cast<T*>(board.get()));
                moves_to_win=cont;
            }   
        }

        return std::make_tuple(value,cont); ;
    }

public:
    //! A constructor.
    /*! This construct an alpha-beta class. A board must be specified together with the
        color of the player that needs to make a move. 
        \param board of class reference to an object of type Board on which a game is being played.
        \param color of enum Board::PlayerColor which needs to make a move
        \param depth this is an optional parameter of default value 4. The higher it becomes, the more precise the move.
    */
    alpha_beta(T& board,Board::PlayerColor color,int depth=4): choosen_board(board), color_of_player(color), max_depth(depth) {
        if(color_of_player==Board::White){
            best_value=std::numeric_limits<float>::max();
        }else{
            best_value=-std::numeric_limits<float>::max();
        }
        moves_to_win=max_depth;
    }

    //! Runs the alpha-beta pruning algorithm.
    /*!
        This function just modify the board given in the constructor of the class.
    */
    void run_alpha_beta(){
        std::unique_ptr<Board> board_ptr;
        board_ptr.reset(&choosen_board);
        run_alpha_beta(board_ptr,color_of_player,-std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),0);
        board_ptr.release();
    }

};

#endif //ALPHA_BETA