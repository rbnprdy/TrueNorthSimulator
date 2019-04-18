# University of Arizona TrueNorth Simulator

This is a software simulation for the TrueNorth FPGA emulation created by Dr. Akoglu's Reconfigurable Computing Lab at the University of Arizona. The original TrueNorth architecture is presented by IBM in ["TrueNorth: Design and Tool Flow of a 65 mW 1 Million Neuron Programmable Neurosynaptic Chip"](http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=7229264&isnumber=7271134) (Akopyan *et al.*). An explination for the simulator can be found in the paper ["Filling the Gaps of the TrueNorth Chip"](FIXME) (Purdy *et al.*).

## Getting Started

The simulator can be built using [CMake](https://cmake.org/install/). In order to build on a Unix systems can be done, clone this repository, enter the repository, and run the following commands:

```
cd build
cmake ..
make
```

This will create an executable called `simulator` which can be used to run simulations.

## Running Simulations

A simple help statement for the simulator can be found by running `./simulator -h`:

```
A software simulation of the TrueNorth architecture.
Usage:
  TrueNorthSimulator [OPTION...] INPUT_FILE_NAME, OUTPUT_FILE_NAME, CONFIGURATION_FILE_NAME, NUM_TICKS

  -i, --input arg        Input file
  -o, --output arg       Output file
  -c, --config arg       Config file
      --ticks arg        Number of ticks to run simulation for
  -t, --trace arg        Trace file
  -r, --report_freq arg  Report frequency (default: 1)
  -h, --help             Print help
```

Two files are necessary to begin a simulation: an input file and a configuration file.

### Input Files

The input file is a JSON file that contains both packets to send in to the simulator and parameters for the cores and neurons to use in the simulation. Example input can be found in `data/example/input.json`.

Packets should be specified as a two dimensional array with with the key `packets` where the `ith` inner array contains the packets for the `ith` tick. A `packet` object has the following keys:

```
{
  "destination_core": an array with two integer elements specifying the coordinates of the destination core,
  "destination_axon": a integer representing the index of the destination axon to send the spike to on the destination core,
  "destination_tick": a integer representing the number of the destination tick of the packet
}
```

Along with the packets should be a list of core objects which represent the cores to use in the simulation. Cores should be specified as a one dimensional array with the key `cores`. A `core` object has the following keys:

```
{
  "axons": an array of integers representing the neuron instructions for each axon,
  "neurons": an array of neuron objects which will be explained below,
  "connections": a two dimensional array of 1s and 0s representing the synaptic crossbar. The `ith` inner array should contain the connections for the `ith` neuron,
  "coordinates": an array with two integer elements specifying the coordinates of the core
}
```

Neuron objects contain the neuron parameters for a single neuron. A `neuron` object has the following keys:

```
{
  "reset_potential": an integer representing the reset potential for the neuron,
  "weights": an array of integers containing the weights for the neuron,
  "leak": an integer representing the leak for the neuron,
  "positive_threshold": an integer representing the positive threshold for the neuron,
  "negative_threshold": an integer representing the negative threshold for the neuron,
  "destination_core": an array with two integer elements specifying the coordinates of the destination core. Coordinates should be specified as an offset from the core that the neuron resides in, and so negative coordinates are acceptable. Coordinates of [0, 0] will send the output to the same core that the neuron resides in,
  "destination_axon": an integer representing the index of the destination axon for this neuron's output spikes,
  "destination_tick": an integer representing the index of the destination tick for this neuron's output spikes,
  "current_potential": an integer representing the starting potential for the neuron,
  "reset_mode": an integer representing the desired reset value for the neuron. Reset modes are explained below
}
```

Currently, we have two reset modes implemented:
- 0: Absolute Reset - If the current potential is greater than or equal to the postivie threshold, it resets to the reset potential. If the current potential is less than the negative threshold, it resets to the negative of the reset potential.
- 1: Linear Reset - If the current potential is greater than or equal to the postivie threshold, the positive threshold gets subtracted from the current potential. If the current potential is less than the negative threshold, the negative threhsold is added to the current potential.

### Configuration Files

A configuration file is another json file which stores the high level for the simulation. The following is a list configuraiton parameters:

```
{
    "num_neurons": the number of neurons per core,
    "num_axons": the number of axons per core,
    "num_cores_x": the number of cores to simulate in the x direction,
    "num_cores_y": the number of cores to simulate in the y direction
    "num_weights": the number of weights per neuron,
    "max_tick_offset": the number of ticks in the future for which a packet can be delivered. This corresponds to the number of rows in the scheduler sram. Note that since ticks are offset by 1, the actual value max value that can be given to a destination tick is 1-max_tick_offset,
    "neuron_block_trace_verbosity": explained below,
    "token_controller_trace_verbosity": explained below,
    "scheduler_trace_verbosity": explained below
}
```

Note that a total number of `num_cores_x`*`num_cores_y` cores will be instantiated for the simulation. Cores not specified in the input file will be given no connections and neurons with a threshold of 1, meaning that neurons on these cores will never spike. 

An example configuration can be found in `config.json`.

### Output Files

The output file which the simulation generates indicates which neurons in each core spike for each tick. Cores which have no spiking neurons for a particular tick will not be printed for that tick.

### Trace Files

In order to help show what is occuring in the simulation, trace files can be generated which provide information about various occurances in the simulation.
In an effort to keep trace files readable, the configuration file offers multiple parameters to control the verbosity of the trace file for certain parts of the simulation. The following is a list of what values will result in what kind of output for specific trace configurations:

* `neuron_block_trace_verbosity`:
  - 1: Logs the leaky-integrate-and-fire operation for each specified neuron at each tick.
  - 2: Logs the axons at which there is a connection and a spike is received for each specified neuron at each tick.
* `token_controller_trace_verbosity`:
  - 1: Logs the spikes received for each specified core at each tick as a binary value.
* `scheduler_trace_verbosity`:
  - 1: Logs the current sram word each time the sram current word is updated.
  - 2: Logs (1) as well as the word and bit being written to every time a write occurs.