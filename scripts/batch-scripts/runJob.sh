#!/bin/sh
git clone https://github.com/noahpy/MolSim-SS24.git -b batch-jobs
cd MolSim-SS24
./scripts/batch-scripts/loadModule.sh
mkdir build && cd build
