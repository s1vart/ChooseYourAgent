#include <iostream>
#include "MatchMap.h"
#include "idMaps.h"

using namespace std;



int main() {
    MatchMap matches("tournaments_stages_matches_games_ids.csv");
    vector<Match> search = matches.searchForMatch("Champions Tour 2023: EMEA Last Chance Qualifier","Playoffs","Knockout Round","Team Heretics");
    vector<Match> searchAll = matches.searchForMatch("Sentinels");
  //  matches.checkTeamComps();
    //    for (auto match : searchAll) {
//        cout << match.matchName << endl;
//    }
    matches.mostPopularTeamComp("Breeze");
    matches.mostPopularTeamComp("Icebox");
}
