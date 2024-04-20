cmake_minimum_required(VERSION 3.26)
project(ChooseYourAgent)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChooseYourAgent main.cpp
        Graph.h
        Match.h
        Game.h
        MatchMap.h
        idMaps.h
)
