#include <cassert>
#include "Game.h"

Game::Game(PlayerType type_of_white, PlayerType type_of_black, std::string name_of_white="The great white player", std::string name_of_black="The great black player") {
    
    players[0].player_color=Board::White;
    players[1].player_color=Board::Black;
    
    players[0].player_type=type_of_white;
    players[1].player_type=type_of_black;

    players[0].player_name=name_of_white;
    players[1].player_name=name_of_black;

}

void Game::play(){
    //The white starts
    bool turn=false;
    
    while(game_over()!=Board::GameOver){
        std::cout<<"This is the result after "<< (turn==0 ? "WHITE": "BLACK" )<<" turn\n"; 
        if(make_a_move(turn)){
            turn=!turn;
        }
    }
    std::cout<<"The winner is player : "<<players[!turn].player_name<<"\n";
}