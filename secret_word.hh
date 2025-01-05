#pragma once

#include "word.hh"

class SecretWord: public Word {
	public:
		SecretWord(std::string w);
		std::set<int> getGreensWith(Word guess);
		std::set<int> getYellowsWith(Word guess);
};
