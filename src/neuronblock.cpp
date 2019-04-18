/// neuronblock.cpp
/// 
/// Created for the University of Arizona Reconfigurable Computing Lab
///
///

#include <string>

#include "neuronblock.h"

void NeuronBlock::integrate(std::vector<int> synaptic_weights, int neuron_instruction) {
	current_potential += synaptic_weights[neuron_instruction];
}

void NeuronBlock::leak(int leak){
	current_potential += leak;
}

bool NeuronBlock::spikes(int positive_threshold){
	return current_potential >= positive_threshold;
}

int NeuronBlock::output_potential(int positive_threshold, int negative_threshold, int reset_potential, int reset_mode) {
	if (current_potential >= positive_threshold) {
		switch (reset_mode) {
			case 0:
				return reset_potential;
			case 1:
				return current_potential - positive_threshold;
			default:
				throw "[ERROR] Reset mode of " + std::to_string(reset_mode) + " out of range of acceptable reset modes.";
				
		}
	} else if (current_potential < negative_threshold) {
		switch (reset_mode) {
			case 0:
				return -reset_potential;
			case 1:
				return current_potential + negative_threshold;
			default:
				throw "[ERROR] Reset mode of " + std::to_string(reset_mode) + " out of range of acceptable reset modes.";
		}
	} else {
		return current_potential;
	}
}
