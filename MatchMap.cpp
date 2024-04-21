#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "MatchMap.h"

using namespace std;

MatchMap::MatchMap(const std::string& filename, matchTypeLookup &idMap) {
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

        match.matchTypeID = idMap.getMatchTypeID(match.tournament,match.stage,match.matchType);

        Game game(match.team1, match.team2, stoi(gameID), map);
        if (matches.find(match.matchID) == matches.end()) { // if match doesn't exist add game 1 and add to map
            match.games.push_back(game);
            matches[match.matchID] = match; // Match ID as key
        }
        else { // if the match already exists add the game to it
            matches[match.matchID].games.push_back(game);
        }
        numGames++;

    }
    this->uniqueIDHashMap = matches;
    // setting our other map so we can get matches through match info / no unique identifier
    setData();
}

int MatchMap::size() {
    return uniqueIDHashMap.size();
}

void MatchMap::setData() {
    for (auto iter : uniqueIDHashMap) {
        // data[tournament][stage][matchType][match name] = match;
        data[iter.second.tournament][iter.second.stage][iter.second.matchType][iter.second.matchName] = iter.second;
    }
}

vector<Match> MatchMap::searchForMatch(string tournament, string stage, string matchType, string team){
    vector<Match> output;
        for (auto iter : data[tournament][stage][matchType]) {
            if (containsSubstring(iter.first, team))
                output.push_back(iter.second);
        }
    return output;
}

bool MatchMap::containsSubstring(const string &str1, const string &str2) {
    // If either string is empty, return false
    if (str1.empty() || str2.empty()) {
        return false;
    }

    // Iterate through the first string
    for (size_t i = 0; i <= str1.length() - str2.length(); ++i) {
        // Check if substring starting at position i matches str2
        if (str1.substr(i, str2.length()) == str2) {
            return true; // Substring found
        }
    }
    return false; // Substring not found
}

vector<Match> MatchMap::searchForMatch(string team) {
    vector<Match> output;
    for (auto iter : uniqueIDHashMap) {
        if (containsSubstring(iter.second.matchName, team))
            output.push_back(iter.second);
    }
    return output;
}
