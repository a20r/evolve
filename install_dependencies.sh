#!/bin/bash
mkdir build_dep

cd build_dep
git clone https://github.com/chutsu/munit.git
git clone https://github.com/chutsu/dbg.git
git clone https://github.com/chutsu/al.git
git clone https://github.com/chutsu/dstruct.git

cd munit
sudo make && sudo make install
cd ..

cd dbg
cmake . && make && sudo make install
cd ..

cd al
cmake . && make && sudo make install
cd ..

cd dstruct
cmake . && make && sudo make install
cd ..

rm -rf build_dep
