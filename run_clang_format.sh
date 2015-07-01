#!/bin/zsh
ls src/*.(h|cpp) | xargs clang-format-3.5 -i
