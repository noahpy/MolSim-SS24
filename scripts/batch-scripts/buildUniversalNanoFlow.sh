#!/bin/sh
rm -rf MolSim-SS24-NanoFlow
git clone https://github.com/noahpy/MolSim-SS24.git MolSim-SS24-NanoFlow -b nanoFlowRunner
cd MolSim-SS24-NanoFlow
./scripts/batch-scripts/loadModule.sh
mkdir build && cd build
cmake ..
make -j 5
