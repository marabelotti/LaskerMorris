#include <memory>
#include <chrono>
#include <stdlib.h>
#include <omp.h>
#include "9MorrisSDL.h"

//!Test suite
/* Run with "make test"
*/

int main(){

    setenv("OMP_NUM_THREADS", "8", true);

    std::cout<<"°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n";
    std::cout<<"First test: OPTIMAL MOVE\n";
    std::cout<<"This is one test to see if the algorithm makes the best move\n";
    std::cout<<"°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n";
    {   
        std::vector<int> state({-1,0,0,-1,0,0,-1,-1,0,-1,-1,0,1,-1,1,-1,0,0,0,1,0,0,0,-1});
        std::vector<int> pawn({3,0,9,0});
    
        Board9Morris board9test(state,Board9Morris::Flying, 14,12,pawn);
        board9test.print();
    
        alpha_beta ab(board9test,Board::White,6);
        auto start = std::chrono::high_resolution_clock::now();
        ab.run_alpha_beta();
    
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        std::cout<<"Computed in "<<duration<<" ms\n";
        
        board9test.print();
        
        assert(board9test.game_on_board_over()==Board::GameOver);
    }
    
}