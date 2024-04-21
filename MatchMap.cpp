#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "MatchMap.h"
#include <algorithm>


using namespace std;

MatchMap::MatchMap(const std::string& filename) {
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

        gamesPerMapHelper(map);

        match.matchTypeID = idMap.getMatchTypeID(match.tournament,match.stage,match.matchType);

        Game game(match.team1, match.team2, stoi(gameID), map);
        if (matches.find(match.matchID) == matches.end()) { // if match doesn't exist add game 1 and add to map
            match.games.emplace(game.map, game);
            matches[match.matchID] = match; // Match ID as key
        }
        else { // if the match already exists add the game to it
            matches[match.matchID].games.emplace(game.map, game);
        }
        numGames++;

    }
    this->uniqueIDHashMap = matches;
    // setting our other map so we can get matches through match info / no unique identifier
    setData();
    addGameData();
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
    if (str1.length() <= str2.length())
        return false;

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
    int numMatches = 0;
    for (auto iter : uniqueIDHashMap) {
        if (containsSubstring(iter.second.matchName, team)) {
            output.push_back(iter.second);
            numMatches++;
        }
    }
    cout << team << " played " << numMatches << " official matches in 2023" << endl;
    return output;
}

void MatchMap::addGameData() {
    std::unordered_map<int, Match> matches;
    std::ifstream file(agentPickCSV);
    std::string line;
    std::getline(file, line);
    int i = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        std::getline(iss, token, ','); // tournament
        string tournament = token;

        std::getline(iss, token, ','); // stage
        string stage = token;

        std::getline(iss, token, ','); // match type
        string matchType = token;

        std::getline(iss, token, ','); // map
        string map = token;

        std::getline(iss, token, ','); // team
        string team = token;

        std::getline(iss, token, ','); // agent picked
        string agent = token;

        std::getline(iss, token, ','); // total wins by map
        string win = token;

        std::getline(iss, token, ','); // total loss by map
        string loss = token;

        std::getline(iss, token, ','); // total maps played
        if (!containsSubstring(stage, "All Stages")) {
            for (auto &iter: data[tournament][stage][matchType]) {
//            i++;
//            cout << tournament << stage << matchType << team << matchType << map << i << endl;
                if (iter.second.games.find(map) != iter.second.games.end()) {
                    if (containsSubstring(iter.second.matchName, team)) {
                        if (iter.second.team2 == team) {

                            iter.second.games[map].team2comp.emplace_back(agent);
                            sortTeamComp(iter.second.games[map].team2comp, map);

                            uniqueIDHashMap[iter.second.matchID].games[map].team2comp.emplace_back(agent);
                            sortTeamComp(uniqueIDHashMap[iter.second.matchID].games[map].team2comp, map);

                            countTeamComps(iter.second.games[map].team2comp, map);

                            if (win == "1") {
                                iter.second.games[map].winningTeam = team;
                                uniqueIDHashMap[iter.second.matchID].games[map].winningTeam = team;
                            }
                        }
                        if (iter.second.team1 == team) {

                            iter.second.games[map].team1comp.emplace_back(agent);
                            sortTeamComp(iter.second.games[map].team1comp, map);

                            uniqueIDHashMap[iter.second.matchID].games[map].team1comp.emplace_back(agent);
                            sortTeamComp(uniqueIDHashMap[iter.second.matchID].games[map].team1comp, map);

                            countTeamComps(iter.second.games[map].team1comp, map);

                            if (win == "1") {
                                iter.second.games[map].winningTeam = team;
                                uniqueIDHashMap[iter.second.matchID].games[map].winningTeam = team;
                            }
                        }
                    }
                }
            }
        }


    }
}

void MatchMap::checkTeamComps() {
    int i = 1;
    for (auto iter : uniqueIDHashMap) {
        for (auto game : iter.second.games) {
            if (game.second.team2comp.size() != 5) {
                cout << "missing agent data error#" << i++ << " in " << iter.second.tournament << " " <<
                     iter.second.stage << " " << iter.second.matchType << " " <<
                     iter.second.matchName << " " << game.second.map << " " << endl;
                cout <<"\t" << game.second.team2
                     << " has a team of less than 5 agents on " << game.second.map << endl;
            }
            if (game.second.team1comp.size() != 5) {
                cout << "missing agent data error#" << i++ << " in " << iter.second.tournament << " " <<
                     iter.second.stage << " " << iter.second.matchType << " " <<
                     iter.second.matchName << " " << game.second.map << " "  << endl;
                cout <<"\t" << game.second.team1
                << " has a team of less than 5 agents on " << game.second.map << endl;
            }

        }
    }
}

void MatchMap::sortTeamComp(vector<string> &teamcompVector, string map) {
    if (teamcompVector.size() == 5)
        std::sort(teamcompVector.begin(), teamcompVector.end());
}

void MatchMap::mostPopularTeamComp(string map) {
    if (teamCompTotalPicks.find(map) == teamCompTotalPicks.end()) {
        cout << map << " was not played during this time period\n";
        return;
    }
    string mostPicked;
    int timesPicked = 0;
    for (auto &teamComp : teamCompTotalPicks[map]) {
        if (teamComp.second > timesPicked) {
            mostPicked = teamComp.first;
            timesPicked = teamComp.second;
        }
    }
    cout << "The most popular team composition on " << map
    << " is " << mostPicked << " played a total of " << timesPicked
    << " times in " << totalGamesPerMap[map] << " games\n" <<
    "\tThis number can be bigger than the number of games because BOTH\n"
    "\tteams are able to pick the same composition\n";
}

void MatchMap::gamesPerMapHelper(const string& map) {
    if (totalGamesPerMap.find(map) == totalGamesPerMap.end()) {
        totalGamesPerMap.emplace(map, 1);
    }
    else {
        totalGamesPerMap[map]++;
    }
}

void MatchMap::countTeamComps(vector<string> &teamcompVector, string map) {
    string agentList;
    if (teamcompVector.size() == 5) {
        for (auto &agent: teamcompVector) {
            agentList += agent + ",";
        }
        if (teamCompTotalPicks[map].find(agentList) == teamCompTotalPicks[map].end())
            teamCompTotalPicks[map].emplace(agentList, 1);
        else
            teamCompTotalPicks[map][agentList]++;
    }
}
