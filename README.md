[![Build Status](https://travis-ci.org/[chutsu]/[evolve].png)](https://travis-ci.org/[chutsu]/[evolve])

#evolve
evolve is a Gentic Algorithm implemented in C. Currently features:

- One dimension bit-string
- Roulette Selection
- One point crossover
- Simple single character mutation

For more information please have a look at the documentation.

## Requirements

- [al](http://github.com/chutsu/al): Algorithm library in C
- [dstruct](http://github.com/chutsu/dstruct): Data structure library in C
- [munit](http://github.com/chutsu/munit): Unit testing macros for C
- [dbg](http://github.com/chutsu/dbg): Debugging macros for C
- cmake (version 2.6 and higher)

## Installation

    cd <path to evolve>
    cmake .  # the dot '.' is important!
    sudo make install

## Licence
LGPL License
Copyright (C) <2013> Chris Choi

This program is free software: you can redistribute it and/or modify it under
the terms of the Lesser GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
