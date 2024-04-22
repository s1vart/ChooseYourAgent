#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "MatchMap.h"
#include <algorithm>
#include <queue>


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
        if (stage != "All Stages") {
            for (auto &iter: data[tournament][stage][matchType]) { // for every match in this section of the tournament
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
    << " is " << mostPicked << "\nwhich was played a total of " << timesPicked
    << " times in " << totalGamesPerMap[map] << " games\n";
    if (timesPicked >= totalGamesPerMap[map])
        cout <<"\tThis number can be bigger than the number of games because BOTH\n"
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

void MatchMap::setPlayerStats() {
    std::ifstream file(playerStatsCSV);
    std::string line;
    std::getline(file, line);
    int i = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Player player;

        std::getline(iss, token, ','); // tournament
        player.tournament = token;

        std::getline(iss, token, ','); // stage
        player.stage = token;

        std::getline(iss, token, ','); // match type
        string matchType = token;

        std::getline(iss, token, ','); // player
        player.name = token;

        std::getline(iss, token, ','); // team
        player.team = token;

        std::getline(iss, token, ','); // agent(s) picked
        player.agents = token;

        std::getline(iss, token, ','); // rounds played
        player.rounds = stoi(token);
        std::getline(iss, token, ','); // rating
        if (!token.empty())
            player.rating = stof(token);
        else {
            player.rating = -1;
        }
        std::getline(iss, token, ','); // Average combat score
        if (!token.empty())
            player.ACS = stoi(token);
        else {
            player.ACS = -1;
        }
        std::getline(iss, token, ','); // k/d ratio

        std::getline(iss, token, ','); // KAST %

        std::getline(iss, token, ','); // ADR

        std::getline(iss, token, ','); // kills per round

        std::getline(iss, token, ','); // assists per round

        std::getline(iss, token, ','); // first kills per round

        std::getline(iss, token, ','); // first deaths per round

        std::getline(iss, token, ','); // headshot %

        std::getline(iss, token, ','); // clutch success %

        std::getline(iss, token, ','); // clutches won/played

        std::getline(iss, token, ','); // max kills in a single map

        std::getline(iss, token, ','); // kills

        std::getline(iss, token, ','); // deaths

        std::getline(iss, token, ','); // assists

        std::getline(iss, token, ','); // first kills

        std::getline(iss, token, ','); // first deaths
        if (player.stage == "All Stages") {
            if (playerStatsMap[player.tournament].find(player.name) == playerStatsMap[player.tournament].end()) {
                playerStatsMap[player.tournament].emplace(player.name, player);
            }
            else {
                if (playerStatsMap[player.tournament].find(player.name)->second.rounds < player.rounds)
                    playerStatsMap[player.tournament].find(player.name)->second = player;
            }
            // yeah yeah

        }
    }
}

struct CompareByRating {
    bool operator()(const Player& p1, const Player& p2) {
        // Note: We want the min heap, so return true if p1's rating is greater than p2's rating
        return p1.rating > p2.rating;
    }
};

vector<Player> MatchMap::kLargestRatings(string& tournament, int k, int minRounds) {
    vector<Player> players;
    for (auto &iter : playerStatsMap[tournament]) {
        players.push_back(iter.second);
    }


    priority_queue<Player, vector<Player>, CompareByRating> minHeap;

    // Push the first k players into the min heap
    int j = 0;
    while (minHeap.size() < k) {
        if (players[j].rounds >= minRounds)
            minHeap.push(players[j]);
        j++;
    }

    // For the rest of the players, if the player's rating is greater than the top player's rating of the min heap, pop the top and push the current player
    for (int i = k; i < players.size(); ++i) {
        if (players[i].rating > minHeap.top().rating && players[i].rounds >= minRounds) {
            minHeap.pop();
            minHeap.push(players[i]);
        }
    }

    // Collect the k largest players from the min heap
    vector<Player> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }

    // Reverse the result to get the players in descending order of rating
    reverse(result.begin(), result.end());
    return result;
}

void MatchMap::topRatedPlayers() {
    vector<Player> players;
    cout << R"(1 - Champions Tour 2023: Americas Last Chance Qualifier
2 - Champions Tour 2023: Pacific Last Chance Qualifier
3 - Champions Tour 2023: EMEA Last Chance Qualifier
4 - Champions Tour 2023: Masters Tokyo
5 - Champions Tour 2023: Americas League
6 - Champions Tour 2023: EMEA League
7 - Champions Tour 2023: Pacific League
8 - Champions Tour 2023: Lock-In Sao Paulo
9 - Valorant Champions 2023
Type an integer 1 - 9 to select a Tournament
)";
    int tournamentChoice;
    cin >> tournamentChoice;

    string tournament;
    if (tournamentChoice == 1)
        tournament = "Champions Tour 2023: Americas Last Chance Qualifier";
    else if (tournamentChoice == 2)
        tournament = "Champions Tour 2023: Pacific Last Chance Qualifier";
    else if (tournamentChoice == 3)
        tournament = "Champions Tour 2023: EMEA Last Chance Qualifier";
    else if (tournamentChoice == 4)
        tournament = "Champions Tour 2023: Masters Tokyo";
    else if (tournamentChoice == 5)
        tournament = "Champions Tour 2023: Americas League";
    else if (tournamentChoice == 6)
        tournament = "Champions Tour 2023: EMEA League";
    else if (tournamentChoice == 7)
        tournament = "Champions Tour 2023: Pacific League";
    else if (tournamentChoice == 8)
        tournament = "Champions Tour 2023: Lock-In Sao Paulo";
    else if (tournamentChoice == 9)
        tournament = "Valorant Champions 2023";

    cout << R"(This function displays the K top rated players from a tournament.
Enter and integer between 1 - 100 for K, or enter -1 to view all
player ratings in a descending order
)";

    int k;
    cin >> k;

    cout << R"(What is the minimum number of rounds to be including in this list?
Please choose an integer between 1 - 515; 150 is reccomended.
)";
    int rounds;
    cin >> rounds;

    if (k == -1) {
        cout << R"(Do you want to use quicksort or heapsort
1 - Quick sort
2- Heap sort
)";

    }
    else if (k >= 1 && k <= 100) {
        players = kLargestRatings(tournament, k, rounds);
    }
    int i = 1;
    for (auto &player : players) {
        cout << i << ". " << player.name << " Rating: " << player.rating << endl;
        i++;
    }

}
