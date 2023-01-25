
#include "Board.h"

using std::endl;
using std::cout;


std::vector<int> Board::Position::adjacent_positions() const{
    return adj_positions;
}

Board::PieceType Board::Position::piece_on_position() const{
    return piece;
}

Board::Board(){
    value=0;
    game_end=NotGameOver;
    std::ifstream file;

    file.open("./graphic/board_structure");

    if (!file.is_open()){
        std::cerr <<  "The file with the Board stucture could not be opened\n"; 
        exit(EXIT_FAILURE);;
    }

    int num_pos;
    int num_edges;

    if(!(file>>num_pos>>num_edges>>num_pieces)){
        std::cerr <<  "The file with the Board stucture is not in the right format\n"; 
        exit(EXIT_FAILURE);;
    }

    pawn_state=std::vector<int>({0,num_pieces,0,num_pieces});

    positions=std::vector<Position>(num_pos);
    int tail;
    int head;

    for(int ed=0; ed<num_edges;ed++){
        if(!(file>>tail>>head)){
            std::cerr <<  "The file with the Board stucture is not in the right format"; 
            exit(EXIT_FAILURE);;
        }
        positions[tail].adj_positions.push_back(head);
        positions[head].adj_positions.push_back(tail);
    }

    file.close();

}

void Board::print() const{
    std::cout<< "PHAWN STATE: ";
    for(auto el=0; el<4;el++ ){
        std::cout<<pawn_state[el]<<" ";
    }
    std::cout<<"\n";
    visual();

}

int Board::on_board(PlayerColor color) const{
    if (color==Black){
        return pawn_state[0];
    }else{
        return pawn_state[2];
    }
}

int Board::still_to_play(PlayerColor color) const{
    if (color==Black){
        return pawn_state[1];
    }else {
        return pawn_state[3];
    }
}  

int Board::out_of_game(PlayerColor color) const{
    if (color==Black){
        return num_pieces-(pawn_state[0]+pawn_state[1]);
    }else {
        return num_pieces-(pawn_state[2]+pawn_state[3]);
    }
}  

float Board::get_valuation() const{
    return value;
}

Board::PieceType Board::give_winner() const{
    return winner;
}

void Board::visual() const{
    for(auto el=0;el< int(positions.size());el++){
        std::cout<<el<<": ";
        for(auto nei: positions[el].adj_positions ){
            std::cout<< nei <<" ";
        }
        std::cout<<"\n";
    }
}

Board::GameEnd Board::game_on_board_over(){
    return game_end;
}   