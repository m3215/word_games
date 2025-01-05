project('word_games', 'cpp', default_options: [ 'cpp_std=c++23', 'optimization=3', 'warning_level=2' ])

# meson doesn't currently support c++23 on Windows through MSVC. by setting cpp_std to cpplatest, we can trick it into loading the most recent version of c++, which should include c++23
if host_machine.system() == 'windows'
    meson.override_option('cpp_std', 'c++latest')
endif

wordle_src_files = files('wordle.cc', 'word.cc', 'secret_word.cc')
hangman_src_files = files('hangman.cc', 'word.cc')

executable('wordle', wordle_src_files)
executable('hangman', hangman_src_files)
