# Qwirkle Group Project ReadMe

## Team members 

| Student Name   | Student Id   | Student Email |
| :------------- | :----------: | :-------------|
| Ashwin Venkita Subharaman | s3783614 | s3783614@student.rmit.edu.au|
| Aidan McGinniss  | s3545999 | s3545999@student.rmit.edu.au|
| Shewit Tikue | s3545853 | s3545853@student.rmit.edu.au|
| Hamida Ehsani | s3653728 | s3653728@student.rmit.edu.au|


</br>
<hr></hr>

## This version includes enhancements by Aidan McGinniss

<hr></hr>

## How to run the program
The main file that runs the program is `qwirkle.cpp`. 

To run this project you will need to compile the entire code, this can be done using the following steps
- `make`
- `./qwirkle`

This will get the game up and running. To use any of the test cases you can do one of two things:
- ***Option 1:*** you will need start the game then select `2` once your on the menu and provide the following command. Type the name of a .save file without the .save.
    - `Tests/<testName>` - the names of the various tests can be found in the `Tests` folder.

- ***Option 2:*** when running the program using `./qwirkle`, you can input the `.input` file as well, this will make the program run itself as the `.input` file provides commands to the program to run the game til the test is completed. This can be done using the following command:
    - `./qwirkle <Tests/<testName>.input`
</br>
<hr></hr>

## Tests
All the tests from this project are load files. This is done to eliminate the randomness of the tile bag when the game begins. This also allows us to set the size for the bag of tiles, how many tiles are on the board, which player's turn it is and how many tiles are contained within the players hands, basically making testing our program a lot simpler.

To compare the outputs of the test cases run the following commands:
```
./qwirkle < Tests/<testname>.input > Tests/<testname>.gameout
diff -w Tests/<testname>.output Tests/<testname>.gameout
```
- **Multiple PlayersTesting** - These are 3 tests which start games with 2, 3 and 4 Players. Using:
    - `./Qwirkle <Tests/2Players.input`
    - `./Qwirkle <Tests/3Players.input`
    - `./Qwirkle <Tests/4Players.input`


- **EOF -** The purpose of this test is to check if the program ends with `>GoodBye` when the program is terminated by the `EOF` character. Since all linux files have end of file characters running this test is super simple the program will run till there is some text in the `.input` file. This is the same case for all the other test cases as well. To run this test run the following command:
    - `./qwirkle <Tests/EOF.input`

- **cornerTesting -** This test contains all tiles places in the 4 corners of the board. This position would not be possible to reach threw a new game and is only for testing purposes. The primary purpose of this test was to check whether placing the tiles in any edge would cause to crash the program. This test case alse checks that the player who first finishes the game is awarded an additional 6 points. To run this test run the following command:
    - `./qwirkle <Tests/cornerTesting.input`

- **doubleQwirkle -** This test basically check the scoring system, both players start off with a score of zero, and the player NATHAN is given the tile that `O5` which can be placed at location `A26` on the board. If the score of NATHAN's score changes to `24` it means the test was successful and there is no error when achieving qwirkle, as `6` points are gained for each line, and `6` for each Quirkle. The file is terminated using the `end of file character`. To run this test run the following command:
    - `./qwirkle <Tests/doubleQwirkle.input`

- **4PlayerGame -** This test runs an entire game this was done primarly to check the multiple levels of vaidation when tiles are placed next to one another. As it can be seen from the test the check methods implemented check whether it is possible to place a tile in all possible directions. This also tests the scoring of many types of moves. To run this test run the following command:
    - `./qwirkle <tests/4PlayerGame.input`

- **QwirkleScore -** This test is very similar to the `doubleQwirkle` test as the whole purpose of this test is to check if placing a tile all your 6 tiles in a row is counted as a qwirkle. To run this test run the following command:
    - `./qwirkle <Tests/QwirkleScore.input`

- **middleQwirkle -** This test is very similar to the `doubleQwirkle` test as the whole purpose of this test is to check if placing a tile in the middle of a row with tiles on either sides will pass the checks required to place a specific tile in that position. To run this test run the following command:
    - `./qwirkle <Tests/middleQwirkle.input`

- **noLegalMove -** This test is done to check if there is any possible legal move for the player whose turn it is and if there are no legal moves for the player and there is no tiles left in the bag the game will proceed to terminate and declare a winner. To run this test run the following command:
    - `./qwirkle <Tests/noLegalMove.input`

- **playerLastTile -** This test is to check what happens when a player runs out of tiles. The expected output of this test is, which ever player runs out of tiles the game would proceed to end and calculate who won the game, in this case player one would win as they placed the tile and earned the most points. To run this test run the following command:
    - `./qwirkle <Tests/player1LastTile.input`
    - `./qwirkle <Tests/player2LastTile.input`
    - `./qwirkle <Tests/player3LastTile.input`
    - `./qwirkle <Tests/player4LastTile.input`

- **replaceTest -**  This test checks whether the `replace` command actually functions properly as when the game is running the players can run a command `replace <tileColour><tileShape>`, if this is run the first tile that matches the the users selection of tile will be removed and added to the back of the bag of tiles, and offer the ability to replace another tile the refill the hand from the front of the bag. To run this test run the following command:
    - `./qwirkle <Tests/replaceTest.input`

- **SGReplace -** This test checks the save game functionality. this test starts off like other tests where there is a presaved game that is being loaded in, after it loads in, mockUser1 replaces the tile `P6` in their hand, then mockUser2 decides to place tile `Y4` at a legal spot on the board. Since its mockUser1 replaced a tile it can be seen that their hand has a new tile. At this stage the game is saved as the file `SGReplaceSaveOutput.save`. This save file actually checks two things one that the save functionality works as intended and the second the replaced tile`P6` is at the back of the entire bag. To run this test run the following command:
    - `./qwirkle <Tests/SGReplace.input`

- **3 player and 4 player tie -** This test basically checks what happens in case of a tie, the expected output when two players get a tie is to congratulate the people drawing for first and bad luck to the rest. A 2 and 3 player tie act similarly so are considered under the same test. To run this test run the following command:
    - `./qwirkle <Tests/3WayDraw.input`
    - `./qwirkle <Tests/4WayDraw.input`

- **tilePlacement -** This test is a bit more elaborate this checks a valid input, in this case the user tries to place tile `Y2` at the location `M11`, the user might think that this is a valid location to place their tile as on either sides are the tiles `R2` and `Y3` since on one end the colour matches up and on the other side the number matches up. However this is not allowed as if the `Y2` tile was placed it would form a line with 3 very different tiles which is against the rules. To run this test run the following command:
    - `./qwirkle <Tests/tilePlacement.input`

- **userInputError -** The whole purpose of this test to check if the error messages provided by the system actully print out to console when the user does something that is not allowed. In this test when the user tries to replace a tile using `Replace` it returns an error message saying `"Incorrect Input!"` and when they try to replace using the correct replace command but with a wrong command it returns a message saying `"That tile is not in your hand!"`. Similarly when the user tries to place a tile randomly on the board it will return an error message saying `"Tile cannot be placed there!"`. This is minor error messages to help the user better understand what to do! To run this test run the following command:
    - `./qwirkle <Tests/userInputError.input`

</br>
<hr></hr>

## Credits
Some of the references that we used were:
- std::vector?, H. and Fide, M., 2021. How to shuffle a std::vector?. [online] Stack Overflow. Available at: <https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector> [Accessed 16 May 2021].