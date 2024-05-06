MolSim Group F
===

![Testing workflow]
(https://github.com/noahpy/MolSim-SS24/actions/workflows/tests.yaml/badge.svg)

## Group members
- [Noah Schlenker](https://github.com/noahpy)
- [Jimin Kim](https://github.com/jimin31)
- [Christian Nix](https://github.com/Chryzl)

## Build instructions
To build the project, run the following commands:
```
mkdir build && cd build
ccmake ..
make
```

## Run instructions
To run the project, run the following command:
```
src/MolSim -d <delta_t> -e <end_t> ../input/<input_file>
```
For more information about arguments and default settings, type:
```
src/MolSim -h
```
## Generate Doxygen documentation
To generate the Doxygen documentation, run the following command:
```
make doc_doxygen
```
This will generate the documentation into the folder `doxys_documentation`.

## Run tests
To run the tests, run the following command:
```
make tests
tests/tests
```

You will get an output along the lines of:
```
[==========] Running 9 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 2 tests from calcForceTest
[ RUN      ] calcForceTest.CompareNaiiveV2
[       OK ] calcForceTest.CompareNaiiveV2 (81 ms)
[ RUN      ] calcForceTest.Timing
[       OK ] calcForceTest.Timing (10077 ms)
[----------] 2 tests from calcForceTest (10159 ms total)

[----------] 7 tests from PContainerTests
[ RUN      ] PContainerTests.particleIterate
[       OK ] PContainerTests.particleIterate (0 ms)
[ RUN      ] PContainerTests.particleItModF
[       OK ] PContainerTests.particleItModF (0 ms)
[ RUN      ] PContainerTests.particleItModX
[       OK ] PContainerTests.particleItModX (0 ms)
[ RUN      ] PContainerTests.particleItModV
[       OK ] PContainerTests.particleItModV (0 ms)
[ RUN      ] PContainerTests.particleItZero
[       OK ] PContainerTests.particleItZero (0 ms)
[ RUN      ] PContainerTests.pairIterators
[       OK ] PContainerTests.pairIterators (0 ms)
[ RUN      ] PContainerTests.pairItZero
[       OK ] PContainerTests.pairItZero (0 ms)
[----------] 7 tests from PContainerTests (0 ms total)

[----------] Global test environment tear-down
[==========] 9 tests from 2 test suites ran. (10159 ms total)
[  PASSED  ] 9 tests.
```
## Project structure
The project is structured as follows:
![Project structure UML](report/report1/res/strategy_long.png)
Note that this is not a perfect UML diagram, but rather a visualization of the broad structure of the project.

**`Simulation`**
- Any kinds of a simulations are represented as a child class of the `Simulation` class
- Any simulation instance defines the `runSim()` function, which uses the I/O classes (`FileReader`, `FileWriter`), the `PhysicsStrategy` class and the model variables passed to it to calculate the simulation through time.
- Child simulations might extend with new model varibales their parent simulations.

**`PhysicsStrategy`**
- Strategy interface to different implementation of strategy functions
- Strategy functions are interchangable / compareable

**`FileWriter`**
- Template method class defining a common interface of different writers

**`FileReader`**
- Template method class defining a common interface of different readers
- Currently not abstract yet, as only one reader is existant

## Folder structure
This section describes the folder strcuture of this project:
- `tests`: tests of the project 
- `src`: source files of the project
- `src/io`: all source files relating to I/O
- `src/models`: all source files relating to classes representing parts of the model
- `src/physics`: all source files relating to physical calculations
- `src/simulation`: all source files relating to the `Simlation` class
- `src/MolSim.cpp`: source file holding main function

