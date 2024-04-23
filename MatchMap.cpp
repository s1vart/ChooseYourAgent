#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "MatchMap.h"
#include <algorithm>
#include <queue>
#include <chrono>
#include <cstring>


using namespace std;

MatchMap::MatchMap(string year, idMaps& idMap) {
    string filename;
    if (year == "2023")
        filename = matchIDCSV2023;
    if (year == "2022")
        filename = matchIDCSV2022;
    if (year == "2021")
        filename = matchIDCSV2021;
    unordered_map<int, Match> matches;
    ifstream file(filename);
    string line;
    getline(file, line);
    int i = 1;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        Match match;

        getline(iss, token, ',');
        match.tournament = token;

        getline(iss, token, ',');
//        cout << token << " ";
//        cout << i << endl;
        match.tournamentID = stoi(token);

        getline(iss, token, ',');
        match.stage = token;

        getline(iss, token, ',');

        match.stageID = stoi(token);

        getline(iss, token, ',');
        match.matchType = token;

        getline(iss, token, ',');
        match.matchName = token;

        match.parseTeams(match.matchName);

        getline(iss, token, ',');

        match.matchID = stoi(token);

        getline(iss, token, ','); // Skip Game ID
        string map = token;
        getline(iss, token, ','); // map
        string gameID = token;

        gamesPerMapHelper(map);
        i++;
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
    addGameData(year);
}

int MatchMap::size() {
    return uniqueIDHashMap.size();
}

void MatchMap::setData() {
    for (auto &iter : uniqueIDHashMap) {
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

void MatchMap::addGameData(string year) {
    string filename;
    unordered_map<int, Match> matches;
    if (year == "2023")
        filename = agentPickCSV2023;
    else if (year == "2022")
        filename = agentPickCSV2022;
    else {
        filename = agentPickCSV2021;
    }
    ifstream file(filename);
    string line;
    getline(file, line);
    int i = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;

        getline(iss, token, ','); // tournament
        string tournament = token;

        getline(iss, token, ','); // stage
        string stage = token;

        getline(iss, token, ','); // match type
        string matchType = token;

        getline(iss, token, ','); // map
        string map = token;

        getline(iss, token, ','); // team
        string team = token;

        getline(iss, token, ','); // agent picked
        string agent = token;

        getline(iss, token, ','); // total wins by map
        string win = token;

        getline(iss, token, ','); // total loss by map
        string loss = token;

        getline(iss, token, ','); // total maps played
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
        sort(teamcompVector.begin(), teamcompVector.end());
}

void MatchMap::mostPopularTeamComp(string year) {
    string map;
    if (year == "2023") {
        cout << R"(What Map would you like to view?
Ascent
Fracture
Haven
Icebox
Lotus
Pearl
Split
Please type the name of map as it is written above (case sensitive)
)";
    }
    else if (year == "2022") {
        cout << R"(What Map would you like to view?
Ascent
Bind
Breeze
Fracture
Haven
Icebox
Split
Please type the name of map as it is written above (case sensitive)
)";
    }
    else if (year == "2021") {
        cout << R"(What Map would you like to view?
Ascent
Bind
Breeze
Fracture
Haven
Icebox
Split
Please type the name of map as it is written above (case sensitive)
)";
    }
    cin >> map;
    if (teamCompTotalPicks.find(map) == teamCompTotalPicks.end()) {
        cout << "Please ensure the map name was spelled correctly\n";
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
        cout <<"**Note** This number can be bigger than the number of games\n"
        "since both teams are able to pick the same composition\n";
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

void MatchMap::setPlayerStats(string year) {
    string filename;
    if (year == "2023")
        filename = playerStatsCSV2023;
    else if (year == "2022")
        filename = playerStatsCSV2022;
    else
        filename = playerStatsCSV2021;
    
    ifstream file(filename);
    string line;
    getline(file, line);
    int i = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        Player player;

        getline(iss, token, ','); // tournament
        player.tournament = token;

        getline(iss, token, ','); // stage
        player.stage = token;

        getline(iss, token, ','); // match type
        string matchType = token;

        getline(iss, token, ','); // player
        player.name = token;

        getline(iss, token, ','); // team
        player.team = token;

        getline(iss, token, ','); // agent(s) picked
        player.agents = token;

        getline(iss, token, ','); // rounds played
        player.rounds = stoi(token);
        getline(iss, token, ','); // rating
        if (!token.empty())
            player.rating = stof(token);
        else {
            player.rating = -1;
        }
        getline(iss, token, ','); // Average combat score
        if (!token.empty())
            player.ACS = stoi(token);
        else {
            player.ACS = -1;
        }
        getline(iss, token, ','); // k/d ratio

        getline(iss, token, ','); // KAST %

        getline(iss, token, ','); // ADR

        getline(iss, token, ','); // kills per round

        getline(iss, token, ','); // assists per round

        getline(iss, token, ','); // first kills per round

        getline(iss, token, ','); // first deaths per round

        getline(iss, token, ','); // headshot %

        getline(iss, token, ','); // clutch success %

        getline(iss, token, ','); // clutches won/played

        getline(iss, token, ','); // max kills in a single map

        getline(iss, token, ','); // kills

        getline(iss, token, ','); // deaths

        getline(iss, token, ','); // assists

        getline(iss, token, ','); // first kills

        getline(iss, token, ','); // first deaths
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
        if (iter.second.rounds >= minRounds)
            players.push_back(iter.second);
    }


    priority_queue<Player, vector<Player>, CompareByRating> minHeap;

    // Push the first k players into the min heap
    if (players.size() < k) {
        for (const auto & player : players) {
            minHeap.push(player);
        }
    }
    else {
        for (int i = 0; i < k; ++i) {
            minHeap.push(players[i]);
        }
        // For the rest of the players, if the player's rating is greater than the top player's rating of the min heap, pop the top and push the current player
        for (int i = k; i < players.size(); ++i) {
            if (players[i].rating > minHeap.top().rating) {
                minHeap.pop();
                minHeap.push(players[i]);
            }
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

vector<Player> MatchMap::HeapSort(string& tournament, int minRounds){
    vector<Player> players;
    for (auto &iter : playerStatsMap[tournament]) {
        if (iter.second.rounds >= minRounds)
            players.push_back(iter.second);
    }


    priority_queue<Player, vector<Player>, CompareByRating> minHeap;


    for (const auto & player : players) {
        minHeap.push(player);
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

void MatchMap::QuickSort(vector<Player>& players, int low, int high){

    if (low < high){
        float pivot = partition(players, low, high);
        QuickSort(players, low, pivot - 1);
        QuickSort(players,pivot + 1, high);
    }

}

int MatchMap::partition(vector<Player>& players, int low, int high){
    //select pivot element
    float pivot = players[low].rating;
    int up = low;
    int down = high;

    while (up < down){
        while (up <= high && players[up].rating >= pivot){
            up++;
        }
        while (players[down].rating < pivot){
            down--;
        }
        if (up < down)
            swap(players[up], players[down]);
    }
    swap(players[low], players[down]);
    return down;
}

void MatchMap::topRatedPlayers23() {
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

    cout << R"(What is the minimum number of rounds played to be included in this list?
Please choose an integer between 1 - 515; 150 is reccomended.
)";
    int rounds;
    cin >> rounds;


    if (k == -1) {
        cout << R"(Do you want to use quicksort or heapsort
1 - Quick sort
2 - Heap sort
)";
        int sorting_algorithm;
        cin >> sorting_algorithm;

        //chrono::high_resolution_clock::time_point start;
        //chrono::high_resolution_clock::time_point stop;
        //chrono::high_resolution_clock::duration duration;

        if (sorting_algorithm == 1){
            //create vector
            vector<Player> players;
            for (auto &iter : playerStatsMap[tournament]) {
                if (iter.second.rounds >= rounds)
                    players.push_back(iter.second);
            }

            auto start = chrono::high_resolution_clock::now();

            QuickSort(players, 0, players.size()-1);

            auto stop = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

            int i = 1;
            for (auto &player : players) {
                cout << i << ". " << player.name << " Rating: " << player.rating << endl;
                i++;


            }

        }
        else if (sorting_algorithm==2) {
            auto start = chrono::high_resolution_clock::now();
            players = HeapSort(tournament, rounds);

            auto stop = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

            cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        }

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

void MatchMap::topRatedPlayers21() {
    vector<Player> players;
    cout << R"(1 - Valorant Champions Tour Stage 3: Masters Berlin
2 - Valorant Champions 2021
3 - Valorant Champions Tour Stage 2: Masters Reykjavík
Type an integer 1 - 3 to select a Tournament
)";

    int tournamentChoice;
    cin >> tournamentChoice;

    string tournament;
    if (tournamentChoice == 1)
        tournament = "Valorant Champions Tour Stage 3: Masters Berlin";
    else if (tournamentChoice == 2)
        tournament = "Valorant Champions 2021";
    else if (tournamentChoice == 3)
        tournament = "Valorant Champions Tour Stage 2: Masters Reykjavík";

    cout << R"(This function displays the K top rated players from a tournament.
Enter and integer between 1 - 100 for K, or enter -1 to view all
player ratings in a descending order
)";

    int k;
    cin >> k;

    cout << R"(What is the minimum number of rounds played to be included in this list?
Please choose an integer between 1 - 200; 150 is reccomended.
)";
    int rounds;
    cin >> rounds;


    if (k == -1) {
        cout << R"(Do you want to use quicksort or heapsort
1 - Quick sort
2 - Heap sort
)";
        int sorting_algorithm;
        cin >> sorting_algorithm;


        if (sorting_algorithm == 1){
            //create vector
            vector<Player> players;
            for (auto &iter : playerStatsMap[tournament]) {
                if (iter.second.rounds >= rounds)
                    players.push_back(iter.second);
            }

            auto start = chrono::high_resolution_clock::now();

            QuickSort(players, 0, players.size()-1);

            auto stop = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

            int i = 1;
            for (auto &player : players) {
                cout << i << ". " << player.name << " Rating: " << player.rating << endl;
                i++;


            }

        }
        else if (sorting_algorithm==2) {
            auto start = chrono::high_resolution_clock::now();
            players = HeapSort(tournament, rounds);

            auto stop = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

            cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        }

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

void MatchMap::topRatedPlayers22() {
    vector<Player> players;
    cout << R"(1 - Valorant Champions Tour Stage 1: Masters Reykjavík
2 - Valorant Champions Tour Stage 2: Masters Copenhagen
3 - Valorant Champions 2022
Type an integer 1 - 3 to select a Tournament
)";

    int tournamentChoice;
    cin >> tournamentChoice;

    string tournament;
    if (tournamentChoice == 1)
        tournament = "Valorant Champions Tour Stage 1: Masters Reykjavík";
    else if (tournamentChoice == 2)
        tournament = "Valorant Champions Tour Stage 2: Masters Copenhagen";
    else if (tournamentChoice == 3)
        tournament = "Valorant Champions 2022";

    cout << R"(This function displays the K top rated players from a tournament.
Enter and integer between 1 - 100 for K, or enter -1 to view all
player ratings in a descending order
)";

    int k;
    cin >> k;

    cout << R"(What is the minimum number of rounds played to be included in this list?
Please choose an integer between 1 - 200; 150 is reccomended.
)";
    int rounds;
    cin >> rounds;


    if (k == -1) {
        cout << R"(Do you want to use quicksort or heapsort
1 - Quick sort
2 - Heap sort
)";
        int sorting_algorithm;
        cin >> sorting_algorithm;


        if (sorting_algorithm == 1){
            //create vector
            vector<Player> players;
            for (auto &iter : playerStatsMap[tournament]) {
                if (iter.second.rounds >= rounds)
                    players.push_back(iter.second);
            }

            auto start = chrono::high_resolution_clock::now();

            QuickSort(players, 0, players.size()-1);

            auto stop = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

            int i = 1;
            for (auto &player : players) {
                cout << i << ". " << player.name << " Rating: " << player.rating << endl;
                i++;


            }

        }
        else if (sorting_algorithm==2) {
            auto start = chrono::high_resolution_clock::now();
            players = HeapSort(tournament, rounds);

            auto stop = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

            cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
        }

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

