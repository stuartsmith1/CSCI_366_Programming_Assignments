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
#include "Client.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    this->player = player;
    this->board_size = board_size;
    string fName = "player_" + std::to_string(player) + ".action_board.json";
    vector<vector< int >> board_array(board_size, vector< int > (board_size, 0));
    remove(fName.c_str());
    ofstream setupArray(fName);
    cereal::JSONOutputArchive setup_archive(setupArray);
    setup_archive(cereal::make_nvp("board", board_array));
    setupArray << "\n}";
    setupArray.close();
}


void Client::fire(unsigned int x, unsigned int y) {
    string fName = "player_" + std::to_string(player) + ".shot.json";
    remove(fName.c_str());
    ofstream setupArray(fName);
    cereal::JSONOutputArchive shot_archive(setupArray);
    shot_archive(cereal::make_nvp("x", x), cereal::make_nvp("y", y));
    setupArray << "\n}";
    setupArray.close();
}


bool Client::result_available() {
    string fName = "player_" + std::to_string(player) + ".result.json";
    ifstream test;
    test.open(fName);
    if (test) {
        return true;
    }
    else{
        return false;
    }
}


int Client::get_result() {
    if(result_available()) {
        string fName = "player_" + std::to_string(player) + ".result.json";
        int result;
        ifstream resultArray(fName);
        cereal::JSONInputArchive result_archive(resultArray);
        result_archive(result);
        resultArray.close();
        remove(fName.c_str());
        if (result == 1 || result == -1 || result == 0) {
            return result;
        } else {
            throw std::invalid_argument("bad result");
        }
    }
    else{
        throw std::invalid_argument( "no result" );
    }
}


void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    string fName = "player_" + std::to_string(player) + ".action_board.json";
    vector<vector< int >> board_array(board_size, vector< int > (board_size));
    ifstream old_board(fName);
    cereal::JSONInputArchive board_archive(old_board);
    board_archive(board_array);
    old_board.close();

    remove(fName.c_str());
    board_array[x][y] = result;
    ofstream new_board(fName);
    cereal::JSONOutputArchive board_archive2(new_board);
    board_archive2(cereal::make_nvp("board", board_array));
    new_board << "\n}";
    new_board.close();
}


string Client::render_action_board(){
}