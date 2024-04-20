#include "Match.h"
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

class MatchMap {
    unordered_map<int, Match> map;
public:
    MatchMap(const std::string& filename) {
        std::unordered_map<int, Match> matches;
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            Match match;

            std::getline(iss, token, ',');
            match.tournament = token;

            std::getline(iss, token, ',');
            match.tournamentID = stoi(token);

            std::getline(iss, token, ',');
            match.stage = token;

            std::getline(iss, token, ',');
            match.stageID = stoi(token);

            std::getline(iss, token, ',');
            match.matchType = token;

            std::getline(iss, token, ',');
            match.matchName = token;
            match.parseTeams(match.matchName);

            std::getline(iss, token, ',');
            match.matchID = stoi(token);

            std::getline(iss, token, ','); // Skip Game ID
            string map = token;
            std::getline(iss, token, ','); // map
            string gameID = token;
            Game game(match.team1, match.team2, stoi(gameID), map);
            if (matches.find(match.matchID) == matches.end()) { // if match doesn't exist add game 1 and add to map
                match.games.push_back(game);
                matches[match.matchID] = match; // Match ID as key
            }
            else { // if the match already exists add the game to it
                matches[match.matchID].games.push_back(game);
            }

        }
        this->map = matches;
    }
    int size() {
        return map.size();
    }
};