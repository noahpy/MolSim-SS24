MolSim Group F
===

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

