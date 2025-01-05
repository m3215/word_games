project('word_games', 'cpp', default_options: [ 'cpp_std=c++latest', 'optimization=3', 'warning_level=2' ])

wordle_src_files = files('wordle.cc', 'word.cc', 'secret_word.cc')
hangman_src_files = files('hangman.cc', 'word.cc')

executable('wordle', wordle_src_files)
executable('hangman', hangman_src_files)
