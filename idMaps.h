#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#pragma once

using namespace std;

struct MatchInfo { // simplified Match struct
    int tournamentID;
    int stageID;
    int matchTypeID;
};

class idMaps {
private:
    unordered_map<string, unordered_map<string, unordered_map<string, MatchInfo>>> data; // Tournament -> Stage -> Match Type -> Match Info

    void storeCSVData(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string tournament, stage, matchType;
            int tournamentID, stageID, matchTypeID;
            char comma;
            ss >> ws; // Skip leading whitespaces
            getline(ss, tournament, ',');
            ss >> tournamentID >> comma;
            getline(ss, stage, ',');
            ss >> stageID >> comma;
            getline(ss, matchType, ',');
            ss >> matchTypeID;

            // Add match info to lookup
            addMatchInfo(tournament, stage, matchType, tournamentID, stageID, matchTypeID);
        }

        file.close();
    }

public:
    idMaps(string match_types_ids) {
        storeCSVData(match_types_ids);
    }
    // Function to add match information
    void addMatchInfo(const string& tournament, const string& stage, const string& matchType, int tournamentID, int stageID, int matchTypeID) {
        data[tournament][stage][matchType] = {tournamentID, stageID, matchTypeID};
    }

    // Function to get Match Type ID
    int getMatchTypeID(const string& tournament, const string& stage, const string& matchType) {
        return data[tournament][stage][matchType].matchTypeID;
    }
};
