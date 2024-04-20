#include <iostream>
#include "MatchMap.h"
#include "idMaps.h"

using namespace std;



int main() {
    matchTypeLookup idMap("tournaments_stages_match_types_ids.csv");
    MatchMap matches("tournaments_stages_matches_games_ids.csv", idMap);
    cout << matches.size() << endl;
    cout << "Match Type ID: " << idMap.getMatchTypeID("Champions Tour 2023: EMEA Last Chance Qualifier", "Playoffs", "Knockout Round") << endl;
}
