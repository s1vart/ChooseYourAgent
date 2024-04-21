#include <iostream>
#include <vector>
#include <unordered_map>
#pragma once

using namespace std;

struct Game {
    Game() {

    }

    int gameID = 0;
    string team1;
    string team2;
    string map;
    string winingTeam;
    vector<string> team1comp;
    vector<string> team2comp;
    Game(string team1, string team2, int gameID, string map) {
        this->team1 = team1;
        this->team2 = team2;
        this->gameID = gameID;
        this->map = map;
    }

};

