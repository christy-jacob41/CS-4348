// Christy Jacob <CXJ170002>
// This program takes in 2 files. One containing a word puzzle and
// another containing the words to search for. If either of the files
// can't be opened an error message is displayed. This program searches for
// the words specified in the second file in the word puzzle that's in the
// first file.

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

// creating an enum with the directions in which words can be found in the word puzzle
enum direction {LEFT_RIGHT, RIGHT_LEFT, DOWN, UP, RIGHT_DOWN, RIGHT_UP, LEFT_DOWN, LEFT_UP};

// declaring const global variables containing the max rows and columns in the puzzle
const int MAX = 50;

struct wordGame
{
	// declaring word game struct variables
	int version;
	int numberRows;
	int numberColumns;
	char puzzle[MAX][MAX];
};

struct wordFind
{
	// declaring word find struct variables
	int row;
	int column;
	int foundCount;
	string word;
	bool found;
	direction where;
};

// function prototypes

// creating word game and word find structs
// find the word that is part of the wordFind structure inside the wordGame structure.
// If the word is found the wordFind structure will be updated.
void findWord(wordGame &game, wordFind &theFind);

// read the puzzle from the input file and update the wordGame structure.
bool readPuzzle(wordGame &game, string inputFileName);

// display the contents of the puzzle
void displayPuzzle(wordGame &game);

// these functions check each direction for the word
void checkLeftRight(wordGame &game, wordFind &theFind, int row, int col);
void checkRightUp(wordGame &game, wordFind &theFind, int row, int col);
void checkRightDown(wordGame &game, wordFind &theFind, int row, int col);
void checkRightLeft(wordGame &game, wordFind &theFind, int row, int col);
void checkLeftDown(wordGame &game, wordFind &theFind, int row, int col);
void checkLeftUp(wordGame &game, wordFind &theFind, int row, int col);
void checkUp(wordGame &game, wordFind &theFind, int row, int col);
void checkDown(wordGame &game, wordFind &theFind, int row, int col);

int main()
{
	// declaring string variables for file names for the puzzle and words you're looking for
	string puzzleFileName, wordsFileName;

	// declaring an ifstream object for the file containing the words you're searching for
	ifstream words;

	// declaring a word game struct for the puzzle
	wordGame puzzle;

	// declaring a wordFind struct for each word you're searching for
	wordFind currentWord;

	// taking in the puzzle file name
	cin >> puzzleFileName;

	// determining if the puzzle could be opened by calling readPuzzle function
	if(readPuzzle(puzzle, puzzleFileName))
	{
		// displaying message saying puzzle was read from puzzle file
		cout << "The puzzle from file \"" << puzzleFileName << "\""<< endl;

		//calling displayPuzzle function to display the puzzle
		displayPuzzle(puzzle);

		//reading in the name of the file containing the words to search for
		cin >> wordsFileName;

		// opening file containing the words to search for and checking if it opened
		words.open(wordsFileName);
		if(words)
		{
			while(words >> currentWord.word)
			{
				//creating temporary string to turn into upper case
				string tempWord =  currentWord.word;

				// for loop changing all the letter to upper case
				for(int letter = 0; letter < static_cast<int>(tempWord.length()); letter++)
				{
					currentWord.word.at(letter) = static_cast<char>(toupper(tempWord.at(letter)));
				}

				// calling findWord function to see if word could be found
				findWord(puzzle, currentWord);

				// if word could be found // print a message
				if(currentWord.found)
				{
					if(currentWord.foundCount==1)
					{// printing what word was found, where it was found, and which direction it was found
						cout << "The word " << currentWord.word << " was found at (" << currentWord.row+1 << ", " << currentWord.column+1 << ") - ";
						if(currentWord.where==LEFT_RIGHT)
						{
							cout << "right" << endl;
						}
						else if(currentWord.where==RIGHT_LEFT)
						{
							cout << "left" << endl;
						}
						else if(currentWord.where==DOWN)
						{
							cout << "down" << endl;
						}
						else if(currentWord.where==UP)
						{
							cout << "up" << endl;
						}
						else if(currentWord.where==RIGHT_DOWN)
						{
							cout << "right/down" << endl;
						}
						else if(currentWord.where==RIGHT_UP)
						{
							cout << "right/up" << endl;
						}
						else if(currentWord.where==LEFT_DOWN)
						{
							cout << "left/down" << endl;
						}
						else
						{
							cout << "left/up" << endl;
						}
					}
					else	// printing how many times a word was found if it was found more than once
					{
						cout << "The word " << currentWord.word << " was found " << currentWord.foundCount << " times" << endl;
					}

				}
				else
				{
					// message if word couldn't be found
					cout << "The word " << currentWord.word << " was not found" << endl;
				}
			}

			// closing the words file
			words.close();
		}
		else
		{
			// error message and quit if file couldn't be opened
			cout << "The puzzle file \"" << wordsFileName << "\" could not be opened or is invalid" << endl;
		}
	}
	else
	{
		// error message if file couldn't be opened
		cout << "The puzzle file \"" << puzzleFileName << "\" could not be opened or is invalid" << endl;
	}

	return 0;
}

// read the puzzle from the input file and update the wordGame structure.
bool readPuzzle(wordGame &game, string inputFileName)
{
	// declaring an ifstream object for the file containing the puzzle
	ifstream puzzle;

	// bool variable for whether or not the rows and columns are valid and puzzle could be opened
	bool validPuzzle;

	// bool to keep track of whether there are enough values which is true at first
	bool enoughValues = true;

	// declaring string variable to get each line
	string line;

	// declaring ints to keep track of row
	int row = 0;

	// setting the value of the wordGame struct to 1
	game.version = 2;

	// opening the puzzle file and checking if it opened or not
	puzzle.open(inputFileName);
	if(puzzle)
	{
		// taking in the number of rows and columns
		puzzle >> game.numberRows >> game.numberColumns;

		// we can use a while loops so we can check for end of file condition
		while(puzzle>>line && row < game.numberRows)
		{
			// going through each letter in the line and storing it in the array
			for(int col = 0; col < game.numberColumns; col++)
			{
				// if not enough values in each column, make enough values false
				if(line.length()!=game.numberColumns)
				{
					enoughValues = false;
				}
				// taking in each char and populating the puzzle array in the word game struct
				game.puzzle[row][col] = line.at(col);
				// making each letter uppercase
				game.puzzle[row][col] = static_cast<char>(toupper(game.puzzle[row][col]));
			}
			// updating the row
			row++;
		}

		// if not enough rows of information, enough values should be false
		if(row!= game.numberRows)
		{
			enoughValues = false;
		}

		// closing the puzzle file
		puzzle.close();

		// returns false if there aren't enough values or row or column values are not in range
		if((!enoughValues) || ((game.numberRows < 1) || (game.numberRows > MAX) || (game.numberColumns < 1) || (game.numberColumns > MAX)))
		{
			validPuzzle = false;
		}
		else
		{
			validPuzzle = true;
		}
	}
	else
	{
		// return false if file couldn't be opened
		validPuzzle = false;
	}

	// returns the validPuzzle bool variable
	return validPuzzle;
}

// display the contents of the puzzle
void displayPuzzle(wordGame &game)
{
	// printing out the puzzle, we can use a for loop since the rows and columns are known
	for(int row = 0; row < game.numberRows; row++)
	{
		for(int col = 0; col < game.numberColumns; col++)
		{
			cout << game.puzzle[row][col];
		}
		// adding a new line after each row
		cout << endl;
	}
}

// creating word game and word find structs
// find the word that is part of the wordFind structure inside the wordGame structure.
// If the word is found the wordFind structure will be updated.
void findWord(wordGame &game, wordFind &theFind)
{
	// initializing whether the current word was found or not to false and the number of times it was found to 0
	theFind.found = false;
	theFind.foundCount = 0;

	// declaring a variable to keep track of word length
	int wordLength = static_cast<int>(theFind.word.length());

	// searching through the puzzle to see if the first letter of the current word is there
	for(int row = 0; row < game.numberRows; row++)
	{
		for(int col = 0; col < game.numberColumns; col++)
		{
			if(theFind.word.at(0) == game.puzzle[row][col])
			{
				// checking to see if it's okay to search right so you won't go out of bounds
				if(wordLength <= (game.numberColumns-col))
				{
					checkLeftRight(game, theFind, row, col);

					//checking to see if it's okay to search up
					if(wordLength <= row+1)
					{
						checkRightUp(game, theFind, row, col);
					}
					// checking to see if it's okay to search down
					if(wordLength <= (game.numberRows-row))
					{

						checkRightDown(game, theFind, row, col);
					}
				}

				//checking to see if it's okay to search left so you won't go out of bounds
				if(wordLength <= col+1)
				{
					checkRightLeft(game, theFind, row, col);

					//checking to see if it's okay to search up
					if(wordLength <= row+1)
					{
						checkLeftUp(game, theFind, row, col);
					}
					// checking to see if it's okay to search down
					if(wordLength <= (game.numberRows-row))
					{
						checkLeftDown(game, theFind, row, col);
					}
				}

				// checking to see if it's okay to search up
				if(wordLength <= row+1)
				{
					checkUp(game, theFind, row, col);
				}

				// checking to see if it's okay to search down
				if(wordLength <= (game.numberRows-row))
				{
					checkDown(game, theFind, row, col);
				}
			}
		}
	}
}

// checks to see if word is found right
void checkLeftRight(wordGame &game, wordFind &theFind, int row, int col)
{
	// creating a temporary word to compare to the current word
	string tempWord;

	// variables to hold the starting row and column
	int realRow = row;
	int realCol = col;

	// putting the first letter into the temporary string and making updates
	tempWord += game.puzzle[row][col];
	col++;

	// while loop gets every letter and puts it in the temporary string
	while(col < game.numberColumns && tempWord.length() < theFind.word.length())
	{
		// putting each letter in the temporary string and making the necessary updates
		tempWord += game.puzzle[row][col];
		col++;
	}

	// checks to see if the word is in the temporary word
	if(tempWord.find(theFind.word)!=string::npos)
	{
		theFind.found = true;
		theFind.foundCount++;
		theFind.where = LEFT_RIGHT;
		theFind.row = realRow;
		theFind.column = realCol;

	}
}

// checks to see if word is found right up
void checkRightUp(wordGame &game, wordFind &theFind, int row, int col)
{
	// creating a temporary word to compare to the current word
	string tempWord;

	// variables to hold the starting row and column
	int realRow = row;
	int realCol = col;

	// putting the first letter into the temporary string and making updates
	tempWord += game.puzzle[row][col];
	col++;
	row--;

	// while loop gets every letter and puts it in the temporary string
	while(col < game.numberColumns && row >= 0 && tempWord.length() < theFind.word.length())
	{
		// putting each letter in the temporary string and making the necessary updates
		tempWord += game.puzzle[row][col];
		col++;
		row--;
	}

	// checks to see if the word is in the temporary word
	if(tempWord.find(theFind.word)!=string::npos)
	{
		theFind.found = true;
		theFind.foundCount++;
		theFind.where = RIGHT_UP;
		theFind.row = realRow;
		theFind.column = realCol;
	}

}

//checks to see if word is found right down
void checkRightDown(wordGame &game, wordFind &theFind, int row, int col)
{
	// creating a temporary word to compare to the current word
	string tempWord;

	// variables to hold the starting row and column
	int realRow = row;
	int realCol = col;

	// putting the first letter into the temporary string and making updates
	tempWord += game.puzzle[row][col];
	col++;
	row++;

	// while loop gets every letter and puts it in the temporary string
	while(col < game.numberColumns && row < game.numberRows && tempWord.length() < theFind.word.length())
	{
		// putting each letter in the temporary string and making the necessary updates
		tempWord += game.puzzle[row][col];
		col++;
		row++;
	}

	// checks to see if the word is in the temporary word
	if(tempWord.find(theFind.word)!=string::npos)
	{
		theFind.found = true;
		theFind.foundCount++;
		theFind.where = RIGHT_DOWN;
		theFind.row = realRow;
		theFind.column = realCol;
	}
}

// checks to see if word is found left
void checkRightLeft(wordGame &game, wordFind &theFind, int row, int col)
{
	// creating a temporary word to compare to the current word
	string tempWord;

	// variables to hold the starting row and column
	int realRow = row;
	int realCol = col;

	// putting the first letter into the temporary string and making updates
	tempWord += game.puzzle[row][col];
	col--;

	// while loop gets every letter and puts it in the temporary string
	while(col >= 0 && tempWord.length() < theFind.word.length())
	{
		// putting each letter in the temporary string and making the necessary updates
		tempWord += game.puzzle[row][col];
		col--;
	}

	// checks to see if the word is in the temporary word
	if(tempWord.find(theFind.word)!=string::npos)
	{
		theFind.found = true;
		theFind.foundCount++;
		theFind.where = RIGHT_LEFT;
		theFind.row = realRow;
		theFind.column = realCol;
	}
}

// checks to see if word is found left down
void checkLeftDown(wordGame &game, wordFind &theFind, int row, int col)
{
	// creating a temporary word to compare to the current word
	string tempWord;

	// variables to hold the starting row and column
	int realRow = row;
	int realCol = col;

	// putting the first letter into the temporary string and making updates
	tempWord += game.puzzle[row][col];
	col--;
	row++;

	// while loop gets every letter and puts it in the temporary string
	while(col >= 0 && row < game.numberRows && tempWord.length() < theFind.word.length())
	{
		// putting each letter in the temporary string and making the necessary updates
		tempWord += game.puzzle[row][col];
		col--;
		row++;
	}

	// checks to see if the word is in the temporary word
	if(tempWord.find(theFind.word)!=string::npos)
	{
		theFind.found = true;
		theFind.foundCount++;
		theFind.where = LEFT_DOWN;
		theFind.row = realRow;
		theFind.column = realCol;
	}
}

// checks to see if word is found left up
void checkLeftUp(wordGame &game, wordFind &theFind, int row, int col)
{
	// creating a temporary word to compare to the current word
	string tempWord;

	// variables to hold the starting row and column
	int realRow = row;
	int realCol = col;

	// putting the first letter into the temporary string and making updates
	tempWord += game.puzzle[row][col];
	col--;
	row--;

	// while loop gets every letter and puts it in the temporary string
	while(col >= 0 && row >=0 && tempWord.length() < theFind.word.length())
	{
		// putting each letter in the temporary string and making the necessary updates
		tempWord += game.puzzle[row][col];
		col--;
		row--;
	}

	// checks to see if the word is in the temporary word
	if(tempWord.find(theFind.word)!=string::npos)
	{
		theFind.found = true;
		theFind.foundCount++;
		theFind.where = LEFT_UP;
		theFind.row = realRow;
		theFind.column = realCol;
	}
}

// checks to see if word is found up
void checkUp(wordGame &game, wordFind &theFind, int row, int col)
{
	// creating a temporary word to compare to the current word
		string tempWord;

		// variables to hold the starting row and column
		int realRow = row;
		int realCol = col;

		// putting the first letter into the temporary string and making updates
		tempWord += game.puzzle[row][col];
		row--;

		// while loop gets every letter and puts it in the temporary string
		while(row >=0 && tempWord.length() < theFind.word.length())
		{
			// putting each letter in the temporary string and making the necessary updates
			tempWord += game.puzzle[row][col];
			row--;
		}

		// checks to see if the word is in the temporary word
		if(tempWord.find(theFind.word)!=string::npos)
		{
			theFind.found = true;
			theFind.foundCount++;
			theFind.where = UP;
			theFind.row = realRow;
			theFind.column = realCol;
		}
}

// checks to see if word is found down
void checkDown(wordGame &game, wordFind &theFind, int row, int col)
{
	// creating a temporary word to compare to the current word
		string tempWord;

		// variables to hold the starting row and column
		int realRow = row;
		int realCol = col;

		// putting the first letter into the temporary string and making updates
		tempWord += game.puzzle[row][col];
		row++;

		// while loop gets every letter and puts it in the temporary string
		while(row < game.numberRows && tempWord.length() < theFind.word.length())
		{
			// putting each letter in the temporary string and making the necessary updates
			tempWord += game.puzzle[row][col];
			row++;
		}

		// checks to see if the word is in the temporary word
		if(tempWord.find(theFind.word)!=string::npos)
		{
			theFind.found = true;
			theFind.foundCount++;
			theFind.where = DOWN;
			theFind.row = realRow;
			theFind.column = realCol;
		}
}
