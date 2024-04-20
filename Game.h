#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Game {
    int gameID;
    string team1;
    string team2;
    string map;
    string win = "none";
    string loss = "none";

    Game(string team1, string team2, int gameID, string map) {
        this->team1 = team1;
        this->team2 = team2;
        this->gameID = gameID;
        this->map = map;
    }

};