/// neuronblock.h
/// 
/// Created for the University of Arizona Reconfigurable Computing Lab
///
///

#ifndef NEURONBLOCK_H
#define NEURONBLOCK_H

#include <vector>

/**
 * @brief Performs the leaky integrate and fire operation for a neuron.
 * 
 */
class NeuronBlock {
	public:
		void integrate(std::vector<int> synaptic_weights, int neuron_instruction);
		void leak(int leak);
		bool spikes(int positive_threshold);
		int output_potential(int positive_threshold, int negative_threshold, int reset_potential, int reset_mode);

		int current_potential;
};

#endif