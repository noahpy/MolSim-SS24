#!/bin/sh
rm -rf MolSim-SS24
git clone https://github.com/noahpy/MolSim-SS24.git -b assignment5
cd MolSim-SS24
./scripts/batch-scripts/loadModule.sh
mkdir build && cd build
cmake ..
make -j 5
