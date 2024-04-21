#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include "Game.h"

using namespace std;

struct Match {
    string tournament;
    int tournamentID;
    string stage;
    int stageID;
    string matchType;
    string matchName;
    int matchID;
    int matchTypeID;
    string team1;
    string team2;
    map<string, Game> games;

    void parseTeams(const std::string& input) {
        // Find the position of "vs" in the input string
        size_t vsPos = input.find("vs");

        // Extract team1 (substring before "vs")
        this->team1 = input.substr(0, vsPos-1);

        // Extract team2 (substring after "vs")
        this->team2 = input.substr(vsPos + 3); // +3 to skip "vs" and the space after it
    }

};