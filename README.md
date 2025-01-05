# A New Paradigm for Word Game Implementation
## Compilation Instructions
### Requirements
- a C++23-compliant compiler:
  + **Windows:** Visual Studio 2022 17.4 or later
  + **MacOS:** Xcode 15.3 or later
  + **Linux:** either
    - GCC 14 or later
    - Clang 17 or later

### Build

Install [Meson](https://mesonbuild.com/SimpleStart.html#installing-meson).

On Windows, the below commands need to be run in the Visual Studio developer tools command prompt in the start menu (search for "x64 Native Tools Command Prompt"). On MacOS and Linux, the commands can be run from any terminal.

`cd` into the project directory.

Before compiling for the first time, run the below to set up the build directory:

```
meson setup build
```

Then to compile, run:

```
meson compile -C build
```

### Run

#### MacOS/Linux
`cd` into `build`, then run:
```sh
./hangman
```
for Hangman or
```sh
./wordle
```
for Wordle.

#### Windows
`cd` into `build`, then run:
```bat
.\hangman.exe
```
for Hangman or
```bat
.\wordle.exe
```
for Wordle.

Both programs rely on their respective word files being in the parent directory of the current working directory, so they should only be run from within the build directory.

## The Word class

For word games, it is more useful to store words upside-down — as a map between each letter and its positions in the word instead of the reverse (as in a string). The word class implements a simple framework for interacting with words structured in this way.

While it isn't necessary to also keep the word in its ordered form as it can be regenerated using the generated map and the word's length, I have chosen to store it as well to optimize for time, instead of space, efficiency.

## The SecretWord class

The SecretWord class inherits the Word class and introduces some extra functions for checking yellows and greens in Wordle. Because these functions are specific to Wordle, they are kept out of the more general-purpose Word class. Keeping SecretWord separate also makes it easier to distinguish between the guess and secret word when processing guesses.

## Hangman

In hangman, the goal is to guess every letter in a given secret word before running out of guesses. In this version, the user gets a total of six incorrect guesses. Correct guesses do not reduce the amount of guesses left.

This implementation relies on the word class discussed above. The main game loop executes as follows:

1. get and verify a guess
2. add the guess to a list of guessed letters
3. check whether the guess is in the word
4. replace the underscores in a working string at the guess's positions in the word if it is
5. and decrement the number of guesses left if it isn't.

## Wordle

In Wordle, the goal is to guess a secret word in six or fewer guesses. The game provides feedback to the user after every guess: the entire guess is bolded to indicate that it's been processed, and every letter will either be

1. highlighted yellow (right letter, wrong position)
2. highlighted green (right letter, right position)
3. or not highlighted at all (wrong letter).

The game will highlight only as many yellows of a letter as there are unshared occurrences of that letter in the secret word. A simple example follows:

```plaintext
The secret word is "aaabb"
The user guesses "bbbcc"
Only the first two 'b's will be highlighted yellow, because there are only two 'b's in the secret word.
```
