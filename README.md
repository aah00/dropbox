# Dropbox
- A simple command line client which takes one directory (the source) as argument,
keeps monitoring changes in that directory, and uploads any change to its server.
- A simple server which takes one directory (the destination) as argument and
receives any change from its client.
- Bonus 1: Optimise data transfer by avoiding uploading the same file multiple times.
- Bonus 2: Optimise data transfer by avoiding uploading the same partial files (files sharing partially the same content) multiple times.
  - https://sourceforge.net/p/syncpath/code/HEAD/tree/trunk/main.cpp#l219
  - https://github.com/rgmann/coral/tree/master/src/common/rsync
  - https://rsync.samba.org/tech_report/node2.html
  - https://rsync.samba.org/tech_report/
  - https://github.com/librsync/librsync/
  - endianness:
    - https://habr.com/en/articles/779176/
  - json
    - https://www.space-research.org/blog/lib_netsockets.html
  - sendfile
    - https://www.reddit.com/r/C_Programming/comments/15nwpve/getting_segfaults_when_transferring_large_files/
    - https://pastebin.com/4kHr94LE
    - https://pastebin.com/wbw05PPW
    - https://medium.com/swlh/linux-zero-copy-using-sendfile-75d2eb56b39b
  - https://github.com/pedro-vicente/lib_netsockets


- Checking:
  - Check filesize and available disk-space:
    However! You should consider first passing the size of the file, as a good practice. This would allow your client to anticipate exactly how much data is coming through the socket, figure out if (for example) there is enough disk space, etc. It allows you to have some sort of sanity-checking before committing to downloading whatever the server tries to send.


## Assumption
- 

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
