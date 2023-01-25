#include <cassert>
#include "Game_9Morris.h"

Game_9Morris::Game_9Morris(PlayerType type_of_white, PlayerType type_of_black, std::string name_of_white, std::string name_of_black) : Game(type_of_white, type_of_black, name_of_white, name_of_black), Board9Morris() {}

Board::GameEnd Game_9Morris::game_over()
{
    return game_end;
}

bool Game_9Morris::make_a_move(bool turn)
{

    assert(game_over() != Board9Morris::GameOver);
    bool move_ok;

    if (players[turn].player_type == Human)
    {
        if (give_game_phase() == Board9Morris::Placing)
        {
            move_ok = make_a_move_placing(turn);
        }
        else if (give_game_phase() == Board9Morris::Moving)
        {
            move_ok = make_a_move_moving(turn);
        }
        else
        {
            if ((turn == 0 and Board9Morris::pawn_state[2] == 3) or (turn == 1 and Board9Morris::pawn_state[0] == 3))
            {
                move_ok = make_a_move_flying(turn);
            }
            else
            {
                move_ok = make_a_move_moving(turn);
            }
        }
    }
    else
    {
        if (give_game_phase() == Board9Morris::Placing)
        {
            alpha_beta<Board9Morris> algorithm(*this, players[turn].player_color, 4);
            algorithm.run_alpha_beta();
        }
        else
        {
            alpha_beta<Board9Morris> algorithm(*this, players[turn].player_color, 6);
            algorithm.run_alpha_beta();
        }
        move_ok = true;
    }
    
    if(move_ok){
        print();
    }
    return move_ok;
}

bool Game_9Morris::make_a_move_placing(bool turn)
{

    std::cout << "Placing phase. " << players[turn].player_name << ", please add pawn: \n";
    int pos;
    while (!(std::cin >> pos) or !values_are_okay(turn, pos))
    {
        std::cout << "Please, enter a valid move\n";
        std::cin.clear();
        std::cin.ignore();
        std::cout << "\n";
    }
    place_piece(pos, players[turn].player_color);

    check_if_mills_and_remove(turn);
    check_game_phase();

    return true;
}

bool Game_9Morris::make_a_move_moving(bool turn)
{

    int pos_in;
    int pos_fin;
    std::cout << "Moving phase. " << players[turn].player_name << ", please move pawn: \n";

    while (!(std::cin >> pos_in >> pos_fin) or !values_are_okay(turn, pos_fin, pos_in))
    {
        std::cout << "Please, enter a valid move\n";
        std::cin.clear();
        std::cin.ignore();
    }
    move_piece(pos_in, pos_fin, players[turn].player_color);

    check_if_mills_and_remove(turn);
    check_game_phase();

    return true;
}

bool Game_9Morris::make_a_move_flying(bool turn)
{

    std::cout << "Flying phase. " << players[turn].player_name << ", please move pawn: \n";

    int pos_in;
    int pos_fin;

    while ((!(std::cin >> pos_in >> pos_fin)) or !values_are_okay(turn, pos_fin, pos_in))
    {
        std::cout << "Please, enter two valid and positions for the move\n";
        std::cin.clear();
        std::cin.ignore();
        std::cout << "\n";
    }

    move_piece(pos_in, pos_fin, players[turn].player_color);

    check_if_mills_and_remove(turn);
    check_game_phase();
    return true;
}

void Game_9Morris::check_if_mills_and_remove(bool turn)
{
    int pos;
    while (fresh_mills != 0)
    {
        std::cout << "You created a mill. " << players[turn].player_name << ", please remove a pawn from the enemy: \n";
        while ((!(std::cin >> pos)) or pos < 0 or pos > 23 or Board9Morris::positions[pos].piece != (Board::PieceType)(int)players[!turn].player_color)
        {
            std::cout << "Please, enter a valid move\n";
            std::cin.clear();
            std::cin.ignore();
        }
        std::cout << "\n";
        remove_piece(pos, players[turn].player_color);
    }
}

bool Game_9Morris::values_are_okay(bool turn, int pos_fin, int pos_in)
{

    bool empty_final_pos = Board9Morris::positions[pos_fin].piece == Board::Empty;
    bool range_fin = pos_fin > -1 and pos_fin < 24;
    if (give_game_phase() == Board9Morris::Placing)
    {
        return empty_final_pos and range_fin;
    }
    else
    {
        bool right_color_pos_in = Board9Morris::positions[pos_in].piece == (Board::PieceType)(int)players[turn].player_color;
        bool range_in = pos_in > -1 and pos_in < 24;
        if (give_game_phase() == Board9Morris::Moving)
        {
            bool adjacent = false;
            for (auto el : Board9Morris::positions[pos_in].adj_positions)
            {
                if (el == pos_fin)
                {
                    adjacent = true;
                }
            }
            return (range_in and range_fin and right_color_pos_in and empty_final_pos and adjacent);
        }
        else
        {
            return (range_in and range_fin and right_color_pos_in and empty_final_pos);
        }
    }
}
