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
//inside build
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
