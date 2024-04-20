#include <iostream>
#include "MatchMap.h"

using namespace std;

int main() {
    MatchMap matches("tournaments_stages_matches_games_ids.csv");
    cout << matches.size() << endl;
}
