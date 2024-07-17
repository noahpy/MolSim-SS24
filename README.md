# MolSim Group F

![Testing workflow](https://github.com/noahpy/MolSim-SS24/actions/workflows/tests.yaml/badge.svg)

The molecular dynamics SS24 code base of Group F.  
You can find the doxygen documentation hosted on [https://noahpy.github.io/MolSim-SS24/](https://noahpy.github.io/MolSim-SS24/).

## Table of Contents

- [MolSim Group F](#molsim-group-f)
  - [Table of Contents](#table-of-contents)
  - [Group members](#group-members)
  - [Quickstart](#quickstart)
    - [Build instructions](#build-instructions)
    - [Run instructions](#run-instructions)
      - [Assignment 1 simulation](#assignment-1-simulation)
      - [Assignment 2 simulation](#assignment-2-simulation)
      - [Assignment 3 simulation](#assignment-3-simulation)
      - [Assignment 4 simulation](#assignment-4-simulation)
    - [Generate Doxygen documentation](#generate-doxygen-documentation)
    - [Run tests](#run-tests)
    - [Run benchmarks](#run-benchmarks)
    - [Format code](#format-code)
    - [Open man page](#open-man-page)
  - [Documentation](#documentation)
    - [Project structure](#project-structure)
    - [Folder structure](#folder-structure)
    - [Input specification](#input-specification)

## Group members

- [Noah Schlenker](https://github.com/noahpy)
- [Jimin Kim](https://github.com/jimin31)
- [Christian Nix](https://github.com/Chryzl)

## Quickstart

### Build instructions

To build the project, run the following commands:

```
mkdir build && cd build
cmake ..
make
```

To build without doxygen and benchmarks run

```
cmake .. -DGENERATE_DOC=OFF  -DBUILD_BENCH=OFF
```

instead.

### Run instructions

To run the project (in general), run the following command:

```
src/MolSim -d <delta_t> -e <end_t> ../input/<input_file>
```

#### Assignment 1 simulation

```
src/MolSim ../input/eingabe-sonne.txt -e 1000
```

#### Assignment 2 simulation

```
src/MolSim ../input/clusters.txt -c -s 1 -d 0.0002 -e 5
```

#### Assignment 3 simulation

Cuboid collision:

```
src/MolSim ../input/assign3.xml -x -s 3
```

Falling drop:

```
src/MolSim ../input/falling_drop.xml -x -s 3
```

#### Assignment 4 simulation

Rayleigh-Taylor instability:

```
src/MolSim ../input/big_rayleigh_taylor.xml -x -s 4
```

Falling drop:

```
src/MolSim ../input/falling_drop_after_equi.xml -x -s 4
```

For more information about arguments and default settings, type:

```
src/MolSim -h
```

or read the [man page](#open-man-page)

### Generate Doxygen documentation

To generate the Doxygen documentation, run the following command:

```
make doc_doxygen
```

This will generate the documentation into the folder `doxys_documentation`.

### Run tests

To build the tests run:

```
cd build
make tests
```

To run the tests, run the following command:

```
tests/tests
```

Or alternatively with ctest:

```
ctest --test-dir tests
```

### Run benchmarks

The benchmarks are run using [Google benchmark](https://github.com/google/benchmark).
Build:

```
cd build
make benchmarks
```

Run:

```
bench/benchmarks
```

### Format code

If your system has clang-format installed, the target `clangformat` will be created. You can then run:

```
make clangformat
```

to format the code.

### Open man page

To see more details on how to use the program, you can look at our man page.
Enter the project root and then run:

```
man ./.molsim.1
```

## Documentation

### Project structure

The project is structured as follows:
![Project structure UML](docs/res/UMl-Structure.svg)
Note that this is not a perfect UML diagram, but rather a visualization of the broad structure of the project. The latest changes done within the scope of assignment 5 are highlighted in orange.

**`Simulation`**

- Any kinds of a simulations are represented as a child class of the `Simulation` class
- Any simulation instance defines the `runSim()` function, which uses the I/O classes (`FileReader`, `FileWriter`), the `PhysicsStrategy` class and the model variables passed to it to calculate the simulation through time.
- Child simulations extend their parent simulation with new model variables.
  - Multiple simulation classes have been added to allow for the functionality required for each assignment. See the following table for all functionality

|                              | Simulation Type | Gravity            | Lennard-Jones-Potential | Linked-Cell        | Boundary Conditions | Analytics          | Multiple different Particles | Thermostat         | Molecules          |
| ---------------------------- | --------------- | ------------------ | ----------------------- | ------------------ | ------------------- | ------------------ | ---------------------------- | ------------------ | ------------------ |
| PlanetSimulation             | 0               | :white_check_mark: | :x:                     | :x:                | :x:                 | :x:                | :x:                          | :x:                | :x:                |
| LennardJonesSimulation       | 1               | :x:                | :white_check_mark:      | :x:                | :x:                 | :x:                | :x:                          | :x:                | :x:                |
| LinkedLennardJonesSimulation | 2               | :x:                | :white_check_mark:      | :white_check_mark: | :x:                 | :x:                | :x:                          | :x:                | :x:                |
| LennardJonesDomainSimulation | 3               | :x:                | :white_check_mark:      | :white_check_mark: | :white_check_mark:  | :white_check_mark: | :x:                          | :x:                | :x:                |
| MixedLJSimulation            | 4               | :white_check_mark: | :white_check_mark:      | :white_check_mark: | :white_check_mark:  | :white_check_mark: | :white_check_mark:           | :white_check_mark: | :x:                |
| MembraneSimulation           | 5               | :white_check_mark: | :white_check_mark:      | :white_check_mark: | :white_check_mark:  | :white_check_mark: | :white_check_mark:           | :white_check_mark: | :white_check_mark: |

**Note on Run-time:** Without the Linked-Cell algorithm the complexity scales in **O(n<sup>2</sup>)**, otherwise in **O(n)**.

**`PhysicsStrategy`**

- Strategy interface allows for interchangable / compareable implementation of the physics
- Realistically, velocities and positions are calculated according to Störmer-Verlet and are not changed (only parallelized later on)
- Interchangeable are the force calculations -> Essentially each simulation has its own implementation of the forces

**`FileWriter`**

- Template method class defining a common interface of different writers
- There are XYZ and VTK writers available

**`FileReader`**

- Template method class defining a common interface of different readers
- Different input types can be specified (see [here in section Input specification](#input-specification))

**`ParticleCluster`**

- Represents a cluster of particles that can be used to initialize the simulation
- Based on an abstract `ParticleCluster` class
- Child classes include `CuboidParticleCluster` and `SphereParticleCluster`

**`CellGrid`**

- The MD-Simulation uses the linked-cell algorithm starting from the LinkedLennardJonesSimulation
- The algorithm scales in **O(n)** relative to the number of particles
- Assumes a certain _cutoff_ distance after which the forces are neglected

**`Thermostat`**

- Used to cool / heat / maintain the temperature of the simulation
- There are two thermostats available
- The classical thermostat will reduce the temperature without regarding the mean velocity of the system, while the individual thermostat does

**`Analyzer`**

- Can be used to analyze the simulation
- It can produce the density and velocity profile of a simulation
- Can be specified with dimensionality

**`ProgressLogger`**

- Will log the progress of the simulation while it is running to inform the user about progress and ETA
- Will update after each percent of the simulation has finished

### Folder structure

This section describes the folder structure of this project ([see UML-Diagram](#project-structure)):

```
MolSim-SS24
├── bench                           Code to benchmark simulation
│   └── results                     Measurements from g-bench runs
│       └── google_benchmark_plot   Result plots
├── cmake
│   └── modules                     Files to find and load packages
├── docs
│   ├── presentation                The latex-files for the presentations
│   ├── report                      The latex-files for the reports
│   └── res                         Images for the documentation
├── input                           Input files used in the assignments
├── libs
│   └── libxsd                      Code for the xsd library
├── results                         Some density-profiles to plot
├── scripts
│   ├── batch-scripts               Scripts to run our simulations on cluster
│   ├── paraview                    Scripts used to help paraview
│   └── plots                       Scripts to create plots
├── src
│   ├── analytics                   Code for ProgressLogger & Analyzer
│   ├── io
│   │   ├── argparse                Code to parse arguments
│   │   ├── fileReader              Code to read input files
│   │   ├── fileWriter              Code to write output files
│   │   ├── xmlparse                Code to parse xml input
│   │   └── xsd                     Code for xsd (xml input)
│   ├── models
│   │   ├── generators              Code to generate clusters
│   │   ├── linked_cell             Code for the linked-cell data structure
│   │   │   └── cell                Code for the cells in the data structure
│   │   └── molecules               Code to generate molecules
│   ├── physics
│   │   ├── boundaryConditions      Code to do boundary conditions
│   │   ├── forceCal                Code to calculate forces
│   │   ├── locationCal             Code to calculate positions
│   │   ├── thermostat              Code for the thermostats
│   │   └── velocityCal             Code to calculate velocities
│   ├── simulation                  Code for the different simulations
│   └── utils                       Utils code (ArrayUtils, ...)
└── tests
    ├── analytics                   Tests for the Analyzer
    ├── data                        Input for the tests
    ├── io                          Tests for IO
    ├── models
    │   ├── generators              Tests for the clusters
    │   └── linkedcell              Tests for the linked-cell Algorithm
    └── physics                     Tests for the physics codes
        └── boundaryConditions      Tests for the boundary conditions
```

- `src/MolSim.cpp`: Is the core file holding main function

### Input specification
