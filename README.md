#evolve
evolve is a Gentic Algorithm implemented in C.

## Requirements

- `cmake` (version 2.6 and higher)

## Installation
#### Generate make files using `cmake`
First navigate into `evolve` root, and generate make files using `cmake` **Note: the dot '.' is important**!

    cd <path to evolve>
    cmake .  
    
where `<path to evolve>` is the path to `evolve`.

#### Install `evolve` to your system
run:

    sudo make install  

to install `evolve` to `/usr/local/include` (default install directory). 

#### Unit tests
Additionally if you would like to compile the unit tests for `evolve` simply do: 
    
    make
    
unit tests will reside in the `bin` dir where you ran make.
    


## Licence
MIT LICENCE  
Copyright (C) <2012> Chris Choi

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

