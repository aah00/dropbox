# Dropbox
- A simple command line client which takes one directory (the source) as argument,
keeps monitoring changes in that directory, and uploads any change to its server.
- A simple server which takes one directory (the destination) as argument and
receives any change from its client.
- Bonus 1: Optimise data transfer by avoiding uploading the same file multiple times.
- Bonus 2: Optimise data transfer by avoiding uploading the same partial files (files sharing partially the same content) multiple times.
  - https://sourceforge.net/p/syncpath/code/HEAD/tree/trunk/main.cpp#l219

## Assumption
- Two teams can have only one running match unless the game is differentiated by home and away basis, i.e.
    - if Norway(home):Spain(away) is already running, then
      - then a new match Norway(home):Spain(away) cannot be started,
      - but Spain(home):Norway(away) can be started
- Start time of a game is automatically initialized, no need to provide start time manually
- There is no end time of a game unless a call is maded to end the game

</br>

# Pre-requisite
- CMake (version 3.17 or newer)
- Make
- gcc/g++ (require c++14 standard)
- GTest utilities

# How to build the library and execute tests
- Move to `build` directory
- Type `cmake ..`
- Then type `make` and static library `libFootballScoreBoardLib.a` shall be created
- To run the test, type `make test`. Alternatively, can run the test executable
  `test_football_scoreboard_lib/FootballScoreboardLib_test`

# Future work
- Add code-coverage tool to check how much of the code is covered by the written tests
- Configure clang-format to enforce code formatting by CI/CD
- Update CMake configuration to add `GTest` as submodule

# Test-run result
![Successful testrun](docs/test_result.png)
