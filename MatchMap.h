#include "Match.h"
#include "idMaps.h"


class MatchMap {
    unordered_map<int, Match> uniqueIDHashMap; // keys are just match IDS
     // works the same as idMaps
    string matchIDCSV = "tournaments_stages_matches_games_ids.csv";
    string matchTypeCSV = "tournaments_stages_match_types_ids.csv";
    string agentPickCSV = "teams_picked_agents.csv";
    int numGames = 0;
    bool containsSubstring(const std::string& str1, const std::string& str2);
    void addGameData();
    idMaps idMap;
    void sortTeamComp(vector<string>&teamcompVector, string map);
    unordered_map<string, unordered_map<string, unordered_map<string, unordered_map<string, Match>>>> data;
    unordered_map<string, unordered_map<string, int>> teamCompTotalPicks;
    unordered_map<string, int> totalGamesPerMap;
    void gamesPerMapHelper(const string& map);

public:
    void checkTeamComps();
    MatchMap(const string &filename);
    int size();
    void setData(); // moves the data from uniqueIDHashMap into more easily searchable format
    vector<Match> searchForMatch(string tournament, string stage, string matchType, string team); // specific match search
    vector<Match> searchForMatch(string team); // find all matches with this team
    void mostPopularTeamComp(string map);
};