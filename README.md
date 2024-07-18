# MolSim Group F

![Testing workflow](https://github.com/noahpy/MolSim-SS24/actions/workflows/tests.yaml/badge.svg)

The molecular dynamics SS24 code base of Group F.  
This is a Particle Simulation program, which outputs data to be then visualized using [ParaView](https://www.paraview.org/).  
You can find the doxygen documentation hosted on [https://noahpy.github.io/MolSim-SS24/](https://noahpy.github.io/MolSim-SS24/).  
You can find a playlist of our simulations on [Youtube](https://www.youtube.com/playlist?list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl).

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
      - [Assignment 5 simulation](#assignment-5-simulation)
    - [Generate Doxygen documentation](#generate-doxygen-documentation)
    - [Run tests](#run-tests)
    - [Run benchmarks](#run-benchmarks)
    - [Format code](#format-code)
    - [Open man page](#open-man-page)
  - [Documentation](#documentation)
    - [Project structure](#project-structure)
    - [Folder structure](#folder-structure)
    - [Input specification](#input-specification)
      - [Compatibility overview](#compatibility-overview)
      - [Cluster (-c)](#cluster--c)
        - [Cluster Input Example](#cluster-input-example)
      - [Ascii-Art (-a)](#ascii-art--a)
        - [Ascii-Art Input Example](#ascii-art-input-example)
      - [XML (-x)](#xml--x)
    - [Performance](#performance)

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

```sh
src/MolSim ../input/<input_file> <program Arguments>
```

The program arguments can be specified as follows

```
-d, --delta_t=VALUE    Set the time step (default: 0.014)
-e, --end_time=VALUE   Set the end time for simulation (default: 2.8)
    --epsilon=VALUE    Set the depth of LJ potential well (default: 5)
    --sigma=VALUE      Set the Zero crossing of LJ potential (default: 1)
-l, --log_level=LEVEL  Set the logging level (default: 3, incompatible with -p)
      - 1                 trace
      - 2                 debug
      - 3                 info
      - 4                 warn
      - 5                 error
      - 6                 critical
      - 7                 off
-c                     Specify that the given input file describes clusters
-a                     Specify that the given input file is of type ascii art
-x                     Specify that the given input file is of type XML
-s, --simtype=VALUE    Specify simulation type (default: 0)
      - 0                 PlanetSimulation
      - 1                 LennardJonesSimulation
      - 2                 LinkedLennardJonesSimulation
      - 3                 LennardJonesDomainSimulation
      - 4                 MixedLJSimulation
      - 5                 MembraneSimulation
-w, --writetype=VALUE  Specify writer type (default: 0, incompatible with -p)
      - 0                 VTK-Writer
      - 1                 XYZ-Writer
      - 2                 XML-Writer
      - 3                 Empty i.e. no output
-p                     Run performance measurements (incompatible with -l, -w)
-P, --parallel         Specify parallel strategy
      - static
      - task
-h, --help             Display help message
```

For more information about arguments and default settings, type:

```sh
src/MolSim -h
```

or read the [man page](#open-man-page)

#### Assignment 1 simulation

```sh
src/MolSim ../input/eingabe-sonne.txt -e 1000
```

See this [YouTube video](https://www.youtube.com/watch?v=E5wFatZMrjY&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

#### Assignment 2 simulation

```sh
src/MolSim ../input/clusters.txt -c -s 1 -d 0.0002 -e 5
```

See this [YouTube video](https://www.youtube.com/watch?v=VvjTVmM5JKw&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

#### Assignment 3 simulation

Cuboid collision:

```sh
src/MolSim ../input/assign3.xml -x -s 3
```

See this [YouTube video](https://www.youtube.com/watch?v=_jjDo9g1zxU&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

Falling drop:

```sh
src/MolSim ../input/falling_drop.xml -x -s 3
```

See this [YouTube video](https://www.youtube.com/watch?v=zIzGzFO7_gg&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

#### Assignment 4 simulation

Big Rayleigh-Taylor instability:

```sh
src/MolSim ../input/big_rayleigh_taylor.xml -x -s 4
```

See this [YouTube video](https://www.youtube.com/watch?v=2bC6tZDAa3Y&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgD) for our result.

Falling drop:

```sh
src/MolSim ../input/falling_drop_after_equi.xml -x -s 4
```

See this [YouTube video](https://www.youtube.com/watch?v=Os7ZwBb_ywY&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for a result of ours.

#### Assignment 5 simulation

Membrane:

```sh
src/MolSim ../input/membrane.xml -x -s 5
```

See this [YouTube video](https://www.youtube.com/watch?v=FDe_RSdYlXM&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

Rayleigh-Taylor instability in 3D:

```sh
src/MolSim ../input/reyleigh_3D.xml -x -s 4
```

See this [YouTube video](https://www.youtube.com/watch?v=U99R1vSeBxQ&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

Nano-scale flow simulation (base):

```sh
src/MolSim ../input/nano_scale_flow.xml -x -s 4
```

See this [YouTube video](https://www.youtube.com/watch?v=-eWISjhgIgA&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

Nano-scale flow simulation (strong wall + rare thermostat update):

```sh
src/MolSim ../input/nano_scale_mods/nano_flow_stronger_wall_weak_thermo.xml -x -s 4
```

See this [YouTube video](https://www.youtube.com/watch?v=yxNYmXJg5r0&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

Nano-scale flow simulation (strong wall with nice profile result):

```sh
src/MolSim ../input/nano_scale_mods/nano_flow_stronger_wall.xml -x -s 4
```

See this [YouTube video](https://www.youtube.com/watch?v=I4h6tjnJVuI&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

Nano-scale flow simulation (with turbulence):

```sh
src/MolSim ../input/nano_scale_mods/nano_flow_turbulence.xml -x -s 4
```

See this [YouTube video](https://www.youtube.com/watch?v=G34H3SCnpW0&list=PL4YeS_XbiJyJYago_F6bgt_HOoO_9fgDl) for our result.

### Generate Doxygen documentation

To generate the Doxygen documentation, run the following command:

```sh
make doc_doxygen
```

This will generate the documentation into the folder `doxys_documentation`.

### Run tests

To build the tests run:

```sh
cd build
make tests
```

To run the tests, run the following command:

```sh
tests/tests
```

Or alternatively with ctest:

```sh
ctest --test-dir tests
```

### Run benchmarks

The benchmarks are run using [Google benchmark](https://github.com/google/benchmark).
Build:

```sh
cd build
make benchmarks
```

Run:

```sh
bench/benchmarks
```

### Format code

If your system has clang-format installed, the target `clangformat` will be created. You can then run:

```sh
make clangformat
```

to format the code.

### Open man page

To see more details on how to use the program, you can look at our man page.
Enter the project root and then run:

```sh
man ./.molsim.1
```

## Documentation

### Project structure

The project is structured as follows:  
![Project structure UML](docs/res/UML-Structure.svg)


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

- `src/MolSim.cpp`: Is the core file holding the main function

### Input specification

#### Compatibility overview

Input type vs. what can be specified in it

|           | Clusters           | Single Points      | Ascii Art          | Program Arguments (s.a. start, end, ...) | Different particle types |
| --------- | ------------------ | ------------------ | ------------------ | ---------------------------------------- | ------------------------ |
| XYZ       | :x:                | :white_check_mark: | :x:                | :x:                                      | :x:                      |
| Cluster   | :white_check_mark: | :recycle:          | :x:                | :x:                                      | :x:                      |
| Ascii Art | :x:                | :recycle:          | :white_check_mark: | :x:                                      | :x:                      |
| XML       | :white_check_mark: | :white_check_mark: | :x:                | :white_check_mark:                       | :white_check_mark:       |

**Note:** :recycle: means you technically can, but probably shouldn't

#### Cluster (-c)

- A cluster file can only code for clusters. By specifying clusters of size 1, a single point could be created
- Lines that start with a `#` are disregarded as comments
- Each line can specify one cluster
- The input can be written to a `.txt` file
- The format is as follows:

```
# For Cuboid Clusters:
C <posx> <posy> <posz> <width> <height> <depth> <spacing> <mass> <meanVel> <vx> <vy> <vz> <Dim. of Brownian motion (2/3)>
# For Sphere Clusters:
S <posx> <posy> <posz> <radius> <sphereDimensions (2/3)> <mass> <meanVel> <vx> <vy> <vz> <Dim. of Brownian motion (2/3)>
```

- Where the specified position is the back, left, bottom corner of the cuboid or center of the sphere

##### Cluster Input Example

```
# Sphere
S 0.0 0.0 0.0 4 2 1.1225 1 0.1 -30 0 0 2
# Cluster:
C 35 15 0 8 8 1 1.125 1 1 10 0 0 2
```

#### Ascii-Art (-a)

- An ascii-Art input file is a `.txt` file
- It specifies a 2D shape of particles i.e. creates particles in the shape of ascii art
- One can use any character in the ascii art as long as it is listed
- The format is as follows:

```
<posx> <posy> <posz>
<vx> <vy> <vz> <mass> <spacing> <meanVel>
<first char to represent particle> <second char to represent particle> ...
<Shape>
```

##### Ascii-Art Input Example

```
0 0 0
0 -10 0 1 1.125 0.1
+ . : - =
++++++++++++++++++++++++++++++++++-         :+++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++-         :+++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++-         :+++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++-         :+++++++++++++++++++++++++++++++++++++++++++++
........:++++++++:........++++++++-         :++++++++:........:++++++++-.........-++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++-         :++++++++          ++++++++:         -++++++++
        .++++++++.        ++++++++===========++++++++          ++++++++:         -++++++++
        .++++++++.        +++++++++++++++++++++++++++          ++++++++:         -++++++++
        .++++++++.        +++++++++++++++++++++++++++          ++++++++:         -++++++++
        .++++++++.        +++++++++++++++++++++++++++          ++++++++:         -++++++++
```

#### XML (-x)

- The XML input file is the recommended input format as it allows for the must broad input specification
- The input must be stored as `.xml` files
- **Note:** Not all parameters that can be specified work with all simulation types. Please refer to [the overview in the project structure](#project-structure)
- The input specification is as follows:

**`Scaffold`**

```XML
<?xml version="1.0" encoding="UTF-8"?>
<simulation xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    ...
</simulation>
```

**`Simulation parameters`**

```XML
<params>
  <delta_t>yourDeltaT</delta_t>
  <end_time>yourEndTime</end_time>
  <output>NameOfOutputFile</output>
  <frequency>FrequencyOfPlotting</frequency>
  <!-- Domain params -->
  <domainOrigin> <!-- Back-Bottom-Left corner of Domain -->
    <x>xCoord</x>
    <y>yCoord</y>
    <z>zCoord</z>
  </domainOrigin>
  <domainSize>
    <x>sizeInXDirection</x>
    <y>sizeInXDirection</y>
    <z>sizeInZDirection</z> <!-- Set to 0 for 2D domain -->
  </domainSize>
  <cutoff>YourCutoffForLinkedCell</cutoff>
  <!-- Boundary params -> Choose periodic, soft_reflective, outflow -->
  <boundaries>
    <!-- For 2D Domain use bound_four tag -->
    <bound_four>yourBoundary</bound_four> <!-- Left Boundary -->
    <bound_four>yourBoundary</bound_four> <!-- Right Boundary -->
    <bound_four>yourBoundary</bound_four> <!-- Top Boundary -->
    <bound_four>yourBoundary</bound_four> <!-- Bottom Boundary -->
    <!-- For 3D Domain use bound_six tag -->
    <bound_six>yourBoundary</bound_six> <!-- Left Boundary -->
    <bound_six>yourBoundary</bound_six> <!-- Right Boundary -->
    <bound_six>yourBoundary</bound_six> <!-- Top Boundary -->
    <bound_six>yourBoundary</bound_six> <!-- Bottom Boundary -->
    <bound_six>yourBoundary</bound_six> <!-- Front Boundary -->
    <bound_six>yourBoundary</bound_six> <!-- Back Boundary -->
  </boundaries>
  <!-- Thermostat params -->
  <thermostat>
      <initialTemp>yourStartingTemperatureInKelvin</initialTemp>
      <thermoFreq>FrequencyOfThermostatApplication</thermoFreq>
      <maxTempDelta>MaximalChangeInTemperatureInKelvin</maxTempDelta>
      <type>ThermostatType</type> <!-- Choose between individual or classic -->
  </thermostat>
  <!-- Gravity params -->
  <gravity>YourGravity</gravity> <!-- F = gravity * particleMass -->
  <!-- Analyzer params -->
  <analysisName>OutPutNameOfAnalysisFiles</analysisName>
  <analysisFreq>FrequencyOfRunningAnalyzer</analysisFreq>
</params>
```

**`Clusters`**

Scaffold:

```XML
<clusters>
  ...
</clusters>
```

Sphere:

```XML
<sphere>
  <center>
    <x>XCoordOfCenter</x>
    <y>YCoordOfCenter</y>
    <z>ZCoordOfCenter</z>
  </center>
  <vel>
    <x>InitialVelInXDirection</x>
    <y>InitialVelInYDirection</y>
    <z>InitialVelInZDirection</z>
  </vel>
  <radius>RadiusOfSphereInParticles</radius>
  <mass>MassOfParticles</mass>
  <sphereDim>DimensionalityOfSphere(2/3)</sphereDim>
  <spacing>MinimalSpacingBetweenParticles</spacing>
  <brownVel>MeanBrownianVelocity</brownVel>
  <brownDim>DimensionalityToApplyBrownianMotionTo</brownDim>
  <ptype>TypeOfTheParticles</ptype>
</sphere>
```

Cuboid:

```XML
<cuboid>
  <pos> <!-- Lower-Left-Bottom Corner -->
    <x>CornerXCoordinate</x>
    <y>CornerXCoordinate</y>
    <z>CornerZCoordinate</z>
  </pos>
  <vel>
    <x>InitialVelInXDirection</x>
    <y>InitialVelInYDirection</y>
    <z>InitialVelInZDirection</z>
  </vel>
  <dim>
    <x>NumberOfParticlesWidth</x>
    <y>NumberOfParticlesHight</y>
    <z>NumberOfParticlesDepth</z>
  </dim>
  <mass>MassOfParticle</mass>
  <spacing>SpacingBetweenParticles</spacing>
  <brownVel>MeanBrownianVelocity</brownVel>
  <brownDim>DimensionsOfBrownianMotion(2/3)</brownDim>
  <ptype>TypeOfParticle</ptype>
</cuboid>
```

**`Single Particles`**

This follows the VTK structure

```XML
<particles>
  <PointData dim="3">
    xOfFirst yOfFirst zOfFirst xOfSecond yOfSecond zOfSecond ...
  </PointData>

  <VelData dim="3">
    xVelOfFirst yVelOfFirst zVelOfFirst xVelOfSecond yVelOfSecond zVelOfSecond ...
  </ValData>

  <ForceData dim="3">
    xForceOfFirst yForceOfFirst zForceOfFirst xForceOfSecond yForceOfSecond zForceOfSecond ...
  </ForceData>

  <OldForceData dim="3">
    xOldForceOfFirst yOldForceOfFirst zOldForceOfFirst xOldForceOfSecond yOldForceOfSecond zOldForceOfSecond ...
  </OldForceData>

  <MassData dim="1">
    massOfFirst massOfSecond ...
  </MassData>

  <TypeData dim="1">
    typeOfFirst typeOfSecond ...
  </TypeData>
</particles>
```

**`Molecules`**

Currently there is only the membrane available as molecule

Scaffold:

```XML
<molecules>
  ...
</molecules>
```

Membrane:

```XML
<membrane>
  <pos> <!-- Lower-Left-Bottom Corner -->
    <x>CornerXCoordinate</x>
    <y>CornerXCoordinate</y>
    <z>CornerZCoordinate</z>
  </pos>
  <vel>
    <x>InitialVelInXDirection</x>
    <y>InitialVelInYDirection</y>
    <z>InitialVelInZDirection</z>
  </vel>
  <dim>
    <x>NumberOfParticlesWidth</x>
    <y>NumberOfParticlesHight</y>
    <z>NumberOfParticlesDepth</z>
  </dim>
  <mass>MassOfParticle</mass>
  <spacing>SpacingBetweenParticles</spacing>
  <brownVel>MeanBrownianVelocity</brownVel>
  <brownDim>DimensionsOfBrownianMotion(2/3)</brownDim>
  <ptype>TypeOfParticle</ptype>
  <equiDist>r0ForMembrane</equiDist>
  <springConst>SpringConstantK</springConst>
</membrane>
```

**`Different Particles`**

Scaffold:

```XML
<ptypes>
  ...
</ptypes>
```

Specify different particle types and their LJ params + fixing them:

```XML
<ptype type="Particle Type">
    <sigma>SigmaValue</sigma>
    <epsilon>EpsilonValue</epsilon>
    <immobile>TrueWillFixateParticleAndNotMoveIt</immobile>
</ptype>
```

**For Examples see Input files**

### Performance
