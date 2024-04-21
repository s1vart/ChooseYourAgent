#include "Match.h"
#include "idMaps.h"


class MatchMap {
    unordered_map<int, Match> uniqueIDHashMap; // keys are just match IDS
    unordered_map<string, unordered_map<string, unordered_map<string, unordered_map<string, Match>>>> data; // works the same as idMaps
    int numGames = 0;
    bool containsSubstring(const std::string& str1, const std::string& str2);
public:
    MatchMap(const string &filename, matchTypeLookup &idMap);
    int size();
    void setData();
    vector<Match> searchForMatch(string tournament, string stage, string matchType, string team); // specific match search
    vector<Match> searchForMatch(string team); // find all matches with this team
};