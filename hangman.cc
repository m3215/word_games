#include <iostream>
#include <fstream>

#include <random>

#include <vector>
#include <unordered_set>

#include <ranges>
#include <algorithm>

#include "word.hh"

#define MAX_GUESSES 6
#define WORD_LENGTH 1

#define WORDS_FILE "../words.txt"

/* the validateGuess function takes three parameters:
 * 1. the guess to validate (s)
 * 2. the length the guess should be (properLength)
 * 3. an unordered_set of guessed letters (guessedLetters)
 *
 * the function checks that the guess is:
 * a) the proper length (ref: 1)
 * b) all letters (ref: 2)
 * c) and not in the unordered_set of guessed letters (ref: 3)
 *
 * it returns a reference code for each of these issues instead of printing to the user to keep all printing in the main loop
 * if the function finds no issues, it returns a reference code of 0 */
int
validateGuess(std::string s, unsigned int properLength, std::unordered_set<char> guessedLetters) {
	if(s.length() != properLength) {
		return 1;
	}
	for(char c : s) {
		if(!std::isalpha(c)) {
			return 2;
		}
	}
	if(guessedLetters.contains(s[0])) {
		return 3;
	}

	return 0;
}

int
main() {
	// create a vector to store the answers
	// vectors are indexed, so they're easier to pick a random value from
	std::vector<std::string> words;

	// open words.txt and check if the file exists
	std::ifstream wordsFile(WORDS_FILE);
	if(!wordsFile) {
		std::clog << "No " << WORDS_FILE << " file found! Exiting." << std::endl;
		return -1;
	}

	// add each word in words.txt to the vector 'words'
	std::string line;
	while(std::getline(wordsFile, line)) {
		words.push_back(line);
	}

	/* set up the random number generator to pick indices within the bounds of the vector 'answers'
	 * each answer has a corresponding index, so by picking an index we're really picking a word */
	std::random_device rd;
	std::uniform_int_distribution<int> range(0, words.size());

	/* the game's loop will only run if the user wants to play
	 * we keep track of that using the below variable
	 * we can assume they want to play because they just opened the program */
	bool wantsToPlay = true;
	while(wantsToPlay) {
		// assign a random word as the secret word
		Word w(words[range(rd)]);

		/* create a working string to keep track of valid guesses, filled with '_' for every letter in the word
		 * this persists throughout each round of the game,
		 * when the user guesses a letter in the word, the positions of that letter previously occupied by '_' are replaced with that letter */
		std::string workingString(w.getLength(), '_');

		// create an unordered set to keep track of all (even incorrect) guessed letters
		// it is faster to check if a letter is in an unordered_set than an array
		std::unordered_set<char> guessedLetters;

		// set up the guess counter and keep track of whether every letter in the secret word has been guessed
		int guessesLeft	= MAX_GUESSES;
		bool guessed	= false;
		while(!guessed and guessesLeft > 0) { // main game loop
			std::string guessString; // set up a buffer string to store the user's inputted guess

			/* output the working string and let the user know how many guesses they have left
			 * this makes clear to them how long the word is when starting
			 * the below code includes a ternary operator to change the output from "guesses" to "guess" when only one guess remains */
			std::cout << workingString << "  (" << guessesLeft << " " << ((guessesLeft != 1) ? "guesses" : "guess") << " remaining.)" << std::endl;

			// get and validate the user's guess
			bool guessIsValid = false;
			while(!guessIsValid) {
				// get a guess
				std::getline(std::cin, guessString);

				// and convert to lowercase
				guessString = guessString
				            | std::ranges::views::transform([](char c){ return std::tolower(c); })
				            | std::ranges::to<std::string>();

				// then verify that it's a valid guess (see validateGuess, above, for the exact conditions)
				switch(validateGuess(guessString, WORD_LENGTH, guessedLetters)) {
					case 1: { // it's the wrong length
						std::cout << "Your can only guess single characters!" << std::endl;
						break;
					}
					case 2: { // it isn't a letter
						std::cout << "You can only guess letters!" << std::endl;
						break;
					}
					case 3: { // it was already guessed
						std::cout << "You already guessed that letter!" << std::endl;
						break;
					}
					default: {
						guessIsValid = true;
						break;
					}
				}
			}

			// make the guess a char so it can be processed by Word::indicesOf
			char guess = guessString[0];

			if(w.contains(guess)) { // check if the guess appears in the word
				// since we know the guess occurs in the word, we substitute the '_'s at all positions where the letter appears with the letter
				for(int i : w.indicesOf(guess)) {
					workingString[i] = guess;
				}
			}
			else { // since the guess isn't in the word, the user now has one less guess; remove one from the count of guesses Left
				guessesLeft--;
			}

			// add this guess to the unordered_set of guessed letters
			guessedLetters.insert(guess);

			/* check if the word has been guessed entirely
			 * once every letter has been guessed, the working string will have the same contents as the secret word; we can check that instead of checking that every letter in the word has been guessed
			 */
			guessed = (w.getString() == workingString);
		}

		if(guessed) { // congratulate the user if they guessed every letter in the word before running out of guessed
			std::cout << workingString << std::endl;
			std::cout << "Congrats. You got it!" << std::endl;
		}
		else { // tell the user what the word was because they didn't guess it in time
			std::cout << "The word was \"" << w.getString() << ".\" Better luck next time!" << std::endl;
		}

		// ask the user if they want to play again
		std::cout << "Want to play again? (Type \"yes\" if you do.):" << std::endl;

		std::string playAgainInput;
		std::getline(std::cin, playAgainInput);

		// convert to lowercase
		playAgainInput = playAgainInput
		               | std::ranges::views::transform([](char c) { return std::tolower(c); })
		               | std::ranges::to<std::string>();

		wantsToPlay = (playAgainInput == "yes");
	}

	// we exited all loops, so we know the user doesn't want to play again
	std::cout << "Thanks for playing. See you next time!" << std::endl;
}
