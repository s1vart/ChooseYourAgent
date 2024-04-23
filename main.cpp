#include <iostream>
#include "MatchMap.h"
#include "idMaps.h"

using namespace std;

void firstPrompt(string year) {
    cout << "What would you like to do?\n" <<
    "1 - View most popular team compositions\n" <<
    "2 - View highest rated players from the " << year << " season\n" <<
    "3 - Quit \n";
}

void secondPrompt(MatchMap& match,string choice, string year) {
    if (choice == "1") {
        match.mostPopularTeamComp(year);
    }
    else if (choice == "2") {
        if (year == "2023")
            match.topRatedPlayers23();
        else if (year == "2022")
            match.topRatedPlayers22();
        else if (year == "2021")
            match.topRatedPlayers21();
    }
    else if (choice == "3")
        return;
    else{
        cout << "Please enter either 1 or 2\n";
        string newChoice;
        cin >> newChoice;
        secondPrompt(match, newChoice, year);
    }
}

int runner() {
    string choice;
    int i = 0;
    cout << R"(Please select what year of VCT you would like to analyze:
1 - 2021
2 - 2022
3 - 2023
Please type an Integer: either 1, 2, or 3
)" ;
    int yearChoice;
    cin >> yearChoice;

    if (yearChoice == 1) {
        idMaps idMap("vct2021/tournaments_stages_match_types_ids.csv");
        MatchMap matches("2021", idMap);
        matches.setPlayerStats("2021");
        firstPrompt("2021");
        cin >> choice;
        secondPrompt(matches, choice, "2021");
    }
    else if (yearChoice == 2) {
        idMaps idMap("vct2022/tournaments_stages_match_types_ids.csv");
        MatchMap matches("2022", idMap);
        matches.setPlayerStats("2022");
        firstPrompt("2022");
        cin >> choice;
        secondPrompt(matches, choice, "2022");
    }
    else if (yearChoice == 3) {
        idMaps idMap("vct2023/tournaments_stages_match_types_ids.csv");
        MatchMap matches("2023", idMap);
        matches.setPlayerStats("2023");
        firstPrompt("2023");
        cin >> choice;
        secondPrompt(matches, choice, "2023");
    }
    return 0;
}

int main() {
    runner();
}
