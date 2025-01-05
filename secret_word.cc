#include "secret_word.hh"

// construct a SecretWord object
SecretWord::SecretWord(std::string w) : Word(w) {
	// since SecretWord inherits everything from Word, and the latter adds no new parameters, we don't need to do anything here other than call the parent constructor.
}

/* get a set of the positions where the given guess matches this SecretWord's characters in both letter and position
 * this algorithm is pretty simple. all we have to do is iterate through each letter and take the intersection of the sets of its positions in this SecretWord and the guess Word */
std::set<int>
SecretWord::getGreensWith(Word guess) {
	std::set<char> sharedLetters = this->getSharedLettersWith(guess);

	// for each letter, find shared positions and add them to our set
	std::set<int> greenIndices;
	for(char c : sharedLetters) {
		std::ranges::set_intersection(this->indicesOf(c), guess.indicesOf(c), std::inserter(greenIndices, greenIndices.end()));
	}

	return greenIndices;
}

/* get a set of the positions where the given guess's characters match characters in this SecretWord but are in the wrong position.
 * this algorithm is more complicated. it still iterates through every character, though. see below for more explanation. */
std::set<int>
SecretWord::getYellowsWith(Word guess) {
	std::set<char> sharedLetters = this->getSharedLettersWith(guess);

	std::set<int> yellowIndices;
	for(char c : sharedLetters) {
		/* wordle requires that the yellows per letter not exceed its unshared occurrences in the secret word
		 * so we need to:
		 * 1. take the difference of the indices of each shared letter in the guess with the secret word, finding all unshared occurrences of each shared letter in the guess
		 * 2. get the size of that difference
		 * 3. calculate the size of the difference of the indices of each shared letter in the secret word with the guess using the result obtained in (2) (see below)
		 * 4. take the minimum of the results obtained in (2) and (3) to evaluate the amount of indices to copy to the set keeping track of yellow indices
		 * 5. copy an amount of indices from the result obtained in (1) equivalent to the result obtained in (4)
		 *
		 * for example:
		 * - our SecretWord is "aaabb"
		 * - you guess "bbbcc"
		 * - if we only did (1), we would highlight the all three 'b's, when we really only want to higlight the first two
		 * - by also doing (2), (3), and (4), we can determine how many 'b's we should highlight */

		std::set<int> indicesInGuess		= guess.indicesOf(c);
		std::set<int> indicesInSecretWord	= this->indicesOf(c);

		std::set<int> notSharedWithSecretWord;
		std::ranges::set_difference(indicesInGuess, indicesInSecretWord, std::inserter(notSharedWithSecretWord, notSharedWithSecretWord.end()));

		/* we don't need to evaluate the set difference between indicesInSecretWord and indicesInGuess because we only care about the size (or cardinality) of that difference, which is easily calculable
		 * the below math explains how we calculate the cardinality of the above set difference given the three above sets:
		 * let A be a finite set which is equivalent to indicesInGuess
		 * let B be another finite set which is equivalent to indicesInSecretWord
		 * |A \ B| is a known quantity (which we derive from notSharedWithSecretWord)
		 *
		 * |A ∩ B| + |A ∖ B| = |A|
		 * |A ∩ B| = |A| - |A ∖ B|
		 *
		 * |A ∩ B| + |B \ A| = |B|
		 * |B \ A| = |B| - |A ∩ B|
		 * |B \ A| = |B| - (|A| - |A \ B|)
		 * |B \ A| = |B| - |A| + |A \ B| */
		int numOccurrencesNotSharedWithSecretWord	= notSharedWithSecretWord.size();
		int numOccurrencesNotSharedWithGuess		= (int) indicesInSecretWord.size() - (int) indicesInGuess.size() + numOccurrencesNotSharedWithSecretWord; // we need to cast the .size() values (which are unsigned int) to int to avoid an integer overflow if indicesInGuess is larger than indicesInSecretWord

		int minOccurrencesOfC = std::min(numOccurrencesNotSharedWithSecretWord, numOccurrencesNotSharedWithGuess);

		yellowIndices.insert(notSharedWithSecretWord.begin(), std::ranges::next(notSharedWithSecretWord.begin(), minOccurrencesOfC));
	}

	return yellowIndices;
}
