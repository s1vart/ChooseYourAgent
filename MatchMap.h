#include "Match.h"
#include "idMaps.h"
#include "Player.h"


class MatchMap {
    unordered_map<int, Match> uniqueIDHashMap; // keys are just match IDS
     // works the same as idMaps
    string matchIDCSV2023 = "vct2023/tournaments_stages_matches_games_ids.csv";
    string matchTypeCSV2023 = "vct2023/tournaments_stages_match_types_ids.csv";
    string agentPickCSV2023 = "vct2023/teams_picked_agents.csv";
    string playerStatsCSV2023 = "vct2023/players_stats.csv";
    string matchIDCSV2022 = "vct2022/tournaments_stages_matches_games_ids.csv";
    string matchTypeCSV2022 = "vct2022/tournaments_stages_match_types_ids.csv";
    string agentPickCSV2022 = "vct2022/teams_picked_agents.csv";
    string playerStatsCSV2022 = "vct2022/players_stats.csv";
    string matchIDCSV2021 = "vct2021/tournaments_stages_matches_games_ids.csv";
    string matchTypeCSV2021 = "vct2021/tournaments_stages_match_types_ids.csv";
    string agentPickCSV2021 = "vct2021/teams_picked_agents.csv";
    string playerStatsCSV2021 = "vct2021/players_stats.csv";
    int numGames = 0;
    bool containsSubstring(const std::string& str1, const std::string& str2);
    void addGameData(string year);
    static void sortTeamComp(vector<string>&teamcompVector, string map);
    unordered_map<string, unordered_map<string, unordered_map<string, unordered_map<string, Match>>>> data;
    unordered_map<string, unordered_map<string, int>> teamCompTotalPicks;
    unordered_map<string, int> totalGamesPerMap;
    unordered_map<string, unordered_map<string , Player>> playerStatsMap;
    void gamesPerMapHelper(const string& map);
    void countTeamComps(vector<string>&teamcompVector, string map);
    vector<Player> kLargestRatings(string& tournament, int k, int minRounds);
    void QuickSort(vector<Player>& players, int low, int high);
    int partition(vector<Player>& players, int low, int high);
    vector<Player> HeapSort(string& tournament, int minRounds);


public:
    void checkTeamComps();
    void setPlayerStats(string year);
    MatchMap(string year, idMaps& idMaps);
    int size();
    void setData(); // moves the data from uniqueIDHashMap into more easily searchable format
    vector<Match> searchForMatch(string tournament, string stage, string matchType, string team); // specific match search
    vector<Match> searchForMatch(string team); // find all matches with this team
    void mostPopularTeamComp(string year);
    void topRatedPlayers23();
    void topRatedPlayers22();
    void topRatedPlayers21();
};