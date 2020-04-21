//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include "common.hpp"
#include "Server.hpp"

#include <string>
#include <cereal/types/vector.hpp>


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}

void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){
    ifstream setup_board_1;
    ifstream setup_board_2;
    setup_board_1.open(p1_setup_board);
    setup_board_2.open(p2_setup_board);
    this->board_size = board_size;
    string line;
    int i = 0;
    while(getline(setup_board_1, line)){
        i++;
    }
    if (this->board_size != i){
        throw std::invalid_argument( "wrong board size" );
    }
    if (strcmp(p1_setup_board.c_str(), "player_1.setup_board.txt") != 0 || strcmp(p2_setup_board.c_str(), "player_2.setup_board.txt") != 0){
        throw std::invalid_argument( "invalid file name" );
    }
    this->p1_setup_board = scan_setup_board(p1_setup_board);
    this->p2_setup_board = scan_setup_board(p2_setup_board);

    setup_board_1.clear();
    setup_board_2.clear();
    setup_board_1.seekg(0, ios::beg);
    setup_board_2.seekg(0, ios::beg);
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if(x>this->board_size || y>this->board_size){
        return 0;
    }
    char shot;
    if (player == 1){
        shot = p1_setup_board->get(x, y);
        if(shot == 1){
            return 1;
        }
        else{
            return -1;
        }

    }
    else if (player == 2){
        shot = p2_setup_board->get(x, y);
        if(shot == 1){
            return 1;
        }
        else{
            return -1;
        }

    }
    else{
        throw std::invalid_argument( "player number out of bounds" );
    }
}

Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){
    BitArray2D *retArray = new BitArray2D(board_size, board_size);
    ifstream board;
    string line;
    char miss = 'j';
    board.open(setup_board_name);
    int i = 0;
    while(getline(board, line)){
        for(int j=0; j<this->board_size; j++){
            if(line.at(j) != miss){
                retArray->set(i, j);
            }
        }
        i++;
    }
    return retArray;
}

int Server::process_shot(unsigned int player) {
    string fName1 = "player_" + std::to_string(player) + ".shot.json";
    unsigned int x, y;
    ifstream shotArray(fName1);
    cereal::JSONInputArchive shot_archive(shotArray);
    shot_archive(x, y);
    shotArray.close();
    int response;
    if(x>(board_size-1) || x<0 || y>(board_size-1) || y<0){
        response = 0;
    }
    else{response = evaluate_shot(player, x, y);}

    string fName2 = "player_" + std::to_string(player) + ".result.json";
    remove(fName1.c_str());
    remove(fName2.c_str());
    ofstream responseArray(fName2);
    cereal::JSONOutputArchive response_archive(responseArray);
    response_archive(cereal::make_nvp("result", response));
    responseArray << "\n}";
    responseArray.close();
    return SHOT_FILE_PROCESSED;
}