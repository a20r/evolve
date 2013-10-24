# evolve [![Build Status](https://travis-ci.org/chutsu/evolve.png)][1]
evolve is a Gentic Algorithm implemented in C. Currently features:

- **Encoding**
    - One dimensional fixed length bit-string
- **Selection**
    - Roulette selection
    - Tournament selection
- **Crossover**
    - One point crossover
    - Two point crossover
- **Mutation**
    - String mutation
    - Alphanumeric mutation
    - Numeric mutation
    - DNA mutation

For more information please have a look at the documentation.



## Requirements

- [munit](http://github.com/chutsu/munit): Unit testing macros for C
- [dbg](http://github.com/chutsu/dbg): Debugging macros for C
- [al](http://github.com/chutsu/al): Algorithm library in C
- [dstruct](http://github.com/chutsu/dstruct): Data structure library in C
- cmake (version 2.6 and higher)



## Installation

    # obtain evolve
    git clone https://github.com/chutsu/evolve.git
    cd evolve

    # install dependencies
    cd scripts
    sudo bash install_dependencies.sh
    cd ..

    # make evolve
    cmake . # the dot '.' is important!
    make



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

[1]: https://travis-ci.org/chutsu/evolve
