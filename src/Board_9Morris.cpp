#include "Board_9Morris.h"

using std::endl;
using std::cout;



Board9Morris::Board9Morris(): Board() {
    game_phase=Placing;

    std::ifstream file;
    file.open("./graphic/possible_mills");

    if (!file.is_open()){
        std::cerr <<  "The file with the structure of the mills could not be opened"; 
        exit(EXIT_FAILURE);
    }

    int a,b,c,d;
    for(int pos=0; pos<24;pos++){
        if(!(file>>a>>b>>c>>d)){ 
            std::cerr<<"The file with the structure of the mills is in the wrong format\n";
             exit(EXIT_FAILURE);
        }
        positions[pos].piece=Empty;
        mills_structure[pos].structure[0]=a;
        mills_structure[pos].structure[1]=b;
        mills_structure[pos].structure[2]=c;
        mills_structure[pos].structure[3]=d;
    }

    file.close();

}

Board9Morris::Board9Morris(std::vector<int> v, Board9Morris::GamePhase phase, int wmov, int bmov,std::vector<int> pstate): Board9Morris(){
    game_phase=phase;
    white_movibility=wmov;
    black_movibility=bmov;
    pawn_state=pstate;
    fresh_mills=0;
    evaluate_board();

    for(int i=0;i<24;i++){
        if(v[i]==0){
            positions[i].piece=Board::Empty;
        }else if(v[i]==1){
            positions[i].piece=Board::Black_piece;
        }else if(v[i]==-1){
            positions[i].piece=Board::White_piece;
        }
    }
}




Board9Morris::GamePhase Board9Morris::give_game_phase() const{
    return game_phase;
}

void Board9Morris::evaluate_board(){
    value=3*(pawn_state[0]+pawn_state[1]-pawn_state[2]-pawn_state[3]);
    value+=0.1*(-white_movibility+black_movibility);
    value+=fresh_mills;
}

int Board9Morris::get_movibility(int i) const{
    int mov=0;
    for(auto el: positions[i].adj_positions){
        if(positions[el].piece== Empty){
            mov=mov+1;
        }
    }
    return mov;
}

void Board9Morris::check_mills(int i){

    Position pos=positions[i];
    Mills mills_at_i=mills_structure[i];
    
    assert(pos.piece!=Empty);
    assert(fresh_mills==0);
    
    PieceType color=pos.piece;

    int x=mills_at_i.structure[0];
    int y=mills_at_i.structure[1];
    
    if(positions[x].piece==color and positions[y].piece==color){
        fresh_mills+=(int) color;
    }

    int z=mills_at_i.structure[2];
    int t=mills_at_i.structure[3];

    if(positions[z].piece==color and positions[t].piece==color){
        fresh_mills+=(int) color;
    }
}

void Board9Morris::visual() const{
    std::cout<< "MOVIBILITY (W,B) : "<<white_movibility<<" "<<black_movibility<<"\n";

cout<<""<<endl;
    cout<<"\t\t\t\t";drop(0);cout<<"___________________________";drop(1);cout<<"____________________________";drop(2);cout<<""<<endl;
    cout<<"\t\t\t\t";drop(0);cout<<"                           ";drop(1);cout<<"                            ";drop(2);cout<<""<<endl;
    cout<<"\t\t\t\t |                              |                              |"<<endl;
    cout<<"\t\t\t\t |                              |                              |"<<endl;
    cout<<"\t\t\t\t |                              |                              |"<<endl;
    cout<<"\t\t\t\t |       ";drop(8);cout<<"__________________";drop(9);cout<<"__________________";drop(10);cout<<"         |"<<endl;
    cout<<"\t\t\t\t |       ";drop(8);cout<<"                  ";drop(9);cout<<"                  ";drop(10);cout<<"         |"<<endl;
    cout<<"\t\t\t\t |        |                     |                    |         |"<<endl;
    cout<<"\t\t\t\t |        |                     |                    |         |"<<endl;
    cout<<"\t\t\t\t |        |                     |                    |         |"<<endl;
    cout<<"\t\t\t\t |        |         ";drop(16);cout<<"_______";drop(17);cout<<"_______";drop(18);cout<<"          |         |"<<endl;
    cout<<"\t\t\t\t |        |         ";drop(16);cout<<"       ";drop(17);cout<<"       ";drop(18);cout<<"          |         |"<<endl;
    cout<<"\t\t\t\t |        |          |                    |          |         |"<<endl; 
    cout<<"\t\t\t\t |        |          |                    |          |         |"<<endl;
    cout<<"\t\t\t\t";drop(7);cout<<"_____  ";drop(15);cout<<" _______";drop(23);cout<<"                  ";drop(19);cout<<"_______";drop(11);cout<<"______";drop(3);cout<<""<<endl;
    cout<<"\t\t\t\t";drop(7);cout<<"     ";drop(15);cout<<"       ";drop(23);cout<<"                  ";drop(19);cout<<"       ";drop(11);cout<<"      ";drop(3);cout<<""<<endl;
    cout<<"\t\t\t\t |        |          |                    |          |         |"<<endl;
    cout<<"\t\t\t\t |        |          |                    |          |         |"<<endl;
    cout<<"\t\t\t\t |        |          |                    |          |         |"<<endl;
    cout<<"\t\t\t\t |        |        ";drop(22);cout<<" _______";drop(21);cout<<" _______";drop(20);cout<<"         |         |"<<endl;
    cout<<"\t\t\t\t |        |        ";drop(22);cout<<"       ";drop(21);cout<<"       ";drop(20);cout<<"           |         |"<<endl;
    cout<<"\t\t\t\t |        |                     |                    |         |"<<endl;
    cout<<"\t\t\t\t |        |                     |                    |         |"<<endl;
    cout<<"\t\t\t\t |        |                     |                    |         |"<<endl;
    cout<<"\t\t\t\t |      ";drop(14);cout<<"__________________";drop(13);cout<<"  __________________";drop(12);cout<<"        |"<<endl;
    cout<<"\t\t\t\t |      ";drop(14);cout<<"                  ";drop(13);cout<<"                  ";drop(12);cout<<"          |"<<endl;
    cout<<"\t\t\t\t |                              |                              |"<<endl;
    cout<<"\t\t\t\t |                              |                              |"<<endl;
    cout<<"\t\t\t\t |                                                             |"<<endl;
    cout<<"\t\t\t\t";drop(6);cout<<"__________________________";drop(5);cout<<"  _____________________________";drop(4);cout<<""<<endl;
    cout<<"\t\t\t\t";drop(6);cout<<"                          ";drop(5);cout<<"                             ";drop(4);cout<<""<<endl;
}

void Board9Morris::drop(int i) const{
    char col;
    if (positions[i].piece==Empty){
        col=' ';
    }
    if (positions[i].piece==Black_piece){
        col='B';
    }
    if (positions[i].piece==White_piece){
        col='W';
    }

    cout<<" "<<col<<" ";
}

void Board9Morris::move_piece(int pos_in,int pos_fin, PlayerColor color){
    
    PieceType color_to_move=(PieceType) ((int) color);

    assert(positions[pos_fin].piece==Empty);
    assert(positions[pos_in].piece==color_to_move);

    //Taking care of movibility for the moving phase
    if(color_to_move==Black_piece){
        black_movibility=black_movibility-get_movibility(pos_in);
    }else{
        white_movibility=white_movibility-get_movibility(pos_in);
    }

    for(auto el: positions[pos_in].adj_positions){
        if(positions[el].piece==White_piece){
            white_movibility+=1;
        }
        if(positions[el].piece==Black_piece){
            black_movibility+=1;
        }
    }

    positions[pos_fin].piece=positions[pos_in].piece;
    positions[pos_in].piece=Empty;

    if(color_to_move==Black_piece){
        black_movibility=black_movibility+get_movibility(pos_fin);
    }else{
        white_movibility=white_movibility+get_movibility(pos_fin);
    }

    for(auto el: positions[pos_fin].adj_positions){
        if(positions[el].piece==White_piece){
            white_movibility-=1;
        }
        if(positions[el].piece==Black_piece){
            black_movibility-=1;
        }
    }

    //Taking care of movibility for the Flying case
    if(game_phase==Flying and pawn_state[0]<=3){
        black_movibility=(give_color_positions(Empty)).size();
    }

    if(game_phase==Flying and pawn_state[2]<=3){
        white_movibility=(give_color_positions(Empty)).size();
    }

    check_mills(pos_fin);
    
    evaluate_board();
}

void Board9Morris::remove_piece(int pos, PlayerColor color){

    PieceType color_to_remove=(PieceType) -((int) color);
    assert(positions[pos].piece==color_to_remove);

    positions[pos].piece=Empty;

    if(color_to_remove==Black_piece){
        black_movibility=black_movibility-get_movibility(pos);
        pawn_state[0]-=1;
    }else{
        white_movibility=white_movibility-get_movibility(pos);
        pawn_state[2]-=1;
    }

    for(auto el: positions[pos].adj_positions){
        if(positions[el].piece==White_piece){
            white_movibility+=1;
        }
        if(positions[el].piece==Black_piece){
            black_movibility+=1;
        }
    }
    
    fresh_mills+=(int) color_to_remove;

}

void Board9Morris::place_piece(int pos, PlayerColor color){
    
    assert(positions[pos].piece==Empty);
    
    PieceType color_to_add=(PieceType)((int) color);
    positions[pos].piece=color_to_add;

    if(color_to_add==Black_piece){
        black_movibility=black_movibility+get_movibility(pos);
        pawn_state[0]+=1;
        pawn_state[1]-=1;
    }else{
        white_movibility=white_movibility+get_movibility(pos);
        pawn_state[2]+=1;
        pawn_state[3]-=1;
    }

    for(auto el: positions[pos].adj_positions){
        if(positions[el].piece==White_piece){
            white_movibility-=1;
        }
        if(positions[el].piece==Black_piece){
            black_movibility-=1;
        }
    }
    check_mills(pos);

    evaluate_board();

}

void Board9Morris::check_game_phase(){

    assert(game_end!=GameOver);

    if(game_phase==Placing and (pawn_state[1]+pawn_state[3]==0)){
        game_phase=Moving;
    }

    if(game_phase==Moving and (pawn_state[0]==3 or pawn_state[2]==3)){
        game_phase=Flying;
    }

    if(pawn_state[3]==0 and (pawn_state[2]<3 or (white_movibility==0 and pawn_state[2]>3))){
        game_end=GameOver;
        winner=Black_piece;
    }

    if(pawn_state[1]==0 and (pawn_state[0]<3 or (black_movibility==0 and pawn_state[0]>3))){
        game_end=GameOver;
        winner=White_piece;
    }
}


std::vector<std::unique_ptr<Board>> Board9Morris::sons(PlayerColor color) const{

    assert(game_end!=GameOver);
    
    if(game_phase==Placing){
        return sons_place(color);
    }else if(game_phase==Moving){
        return sons_move(color);
    }else{
        assert(game_phase==Flying);
        if((color==Black and pawn_state[0]>2) or (color==White and pawn_state[2]>2)){
                return sons_move(color);
        }else{
            return sons_fly(color);
        }
    }
}

std::vector<std::unique_ptr<Board>> Board9Morris::sons_place(PlayerColor color) const{

    std::vector<std::unique_ptr<Board>> board_sons;

    for(auto el: give_color_positions(Empty)){
        Board9Morris son=*this;
        //This is the function where the board is evaluated.
        son.place_piece(el,color);
        if(abs(son.fresh_mills)==1){
            auto positions_of_enemy=give_color_positions((PieceType) -(int) color);
            for(auto el_rem: positions_of_enemy){
                Board9Morris son_rem=son;
                //std::cout<<"Removing: "<<el_rem<<" from color "<<(int) color;
                son_rem.remove_piece(el_rem,color);
                assert(son_rem.fresh_mills==0);
                son_rem.check_game_phase();
                board_sons.push_back(std::move(std::unique_ptr<Board>(new Board9Morris(son_rem))));
            }
        }else if(abs(son.fresh_mills)==2){
            auto positions_of_enemy=give_color_positions((PieceType) -(int) color);
            for(auto i=0;i< positions_of_enemy.size();i++){
                for(auto j=i+1;j<positions_of_enemy.size();j++){
                    Board9Morris son_rem=son;
                    //std::cout<<"Removing: "<<i<<" and "<<j<<" from color "<<(int) color;
                    son_rem.remove_piece(positions_of_enemy[i],color);
                    son_rem.remove_piece(positions_of_enemy[j],color);
                    assert(son_rem.fresh_mills==0);
                    son_rem.check_game_phase();
                    board_sons.push_back(std::move(std::unique_ptr<Board>(new Board9Morris(son_rem))));
                }
            }
        }else{
            assert(son.fresh_mills==0);
            son.check_game_phase();
            std::unique_ptr<Board> unq_ptr(new Board9Morris(son));
            board_sons.push_back(std::move(unq_ptr));
        }
    }

    return board_sons;

}

std::vector<std::unique_ptr<Board>> Board9Morris::sons_move(PlayerColor color) const{

    std::vector<std::unique_ptr<Board>> board_sons;

    for(auto el: give_color_positions((PieceType) (int) color)){
        for(auto adj: positions[el].adj_positions){
            if(positions[adj].piece==Empty){
                Board9Morris son=*this;
                son.move_piece(el,adj,color);
                if(abs(son.fresh_mills)==1){
                    auto positions_of_enemy=give_color_positions((PieceType) -(int) color);
                    for(auto el_rem: positions_of_enemy){
                        Board9Morris son_rem=son;
                        son_rem.remove_piece(el_rem,color);
                        assert(son_rem.fresh_mills==0);
                        son_rem.check_game_phase();
                        board_sons.push_back(std::unique_ptr<Board>(new Board9Morris(son_rem)));
                    }
                }else{
                    son.check_game_phase();
                    board_sons.push_back(std::unique_ptr<Board>(new Board9Morris(son)));
                }
            }
        }       
    }

    return board_sons;
}

std::vector<std::unique_ptr<Board>> Board9Morris::sons_fly(PlayerColor color) const{

    std::vector<std::unique_ptr<Board>> board_sons;

    for(auto el: give_color_positions((PieceType) (int) color)){
        for(auto empty_pos: give_color_positions(Empty)){
            Board9Morris son=*this;
            son.move_piece(el,empty_pos,color);
            if(abs(son.fresh_mills)==1){
                auto positions_of_enemy=give_color_positions((PieceType) -(int) color);
                for(auto el_rem: positions_of_enemy){
                    Board9Morris son_rem=son;
                    son_rem.remove_piece(el_rem,color);
                    son_rem.check_game_phase();
                    assert(son_rem.fresh_mills==0);
                    board_sons.push_back(std::unique_ptr<Board>(new Board9Morris(son_rem)));
                }
            }else if(abs(son.fresh_mills)==2){
                auto positions_of_enemy=give_color_positions((PieceType) -(int) color);
                for(auto i=0;i< positions_of_enemy.size();i++){
                    for(auto j=i+1;j<positions_of_enemy.size();j++){
                        Board9Morris son_rem=son;
                        son_rem.remove_piece(positions_of_enemy[i],color);
                        son_rem.remove_piece(positions_of_enemy[j],color);
                        assert(son_rem.fresh_mills==0);
                        son_rem.check_game_phase();
                        board_sons.push_back(std::unique_ptr<Board>(new Board9Morris(son_rem)));
                    }
                }
            }else{
                assert(son.fresh_mills==0);
                son.check_game_phase();
                board_sons.push_back(std::unique_ptr<Board>(new Board9Morris(son)));
            }
        }
        
    }

    return board_sons;

}


std::vector<int> Board9Morris::give_color_positions(PieceType color) const{
    std::vector<int> color_positions;

    for(auto el=0; el<24;el++){
        if(positions[el].piece==color){
            color_positions.push_back(el);
        }
    }

    return color_positions;
}

