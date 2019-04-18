/// TrueNorth.cpp
/// 
/// Created for the University of Arizona Reconfigurable Computing Lab
///
///

#include <iostream>

#include <plog/Log.h>

#include "config.hpp"
#include "truenorthgrid.h"
#include "router.h"
#include "scheduler.h"
#include "neuronblock.h"
#include "csramrow.h"
#include "tokencontroller.h"

TrueNorthGrid::TrueNorthGrid(std::vector<std::vector<Packet*>> input_packets, std::vector<Core*> cores) {
	this->cores = cores;
	this->input_packets = input_packets;
}

void TrueNorthGrid::beginActivity(int num_ticks, int report_frequency) {
	std::cout << "Starting simulation with " << num_ticks << " ticks." << std::endl;
	
	if (Config::traceSpecified()) {
		LOG_DEBUG_(1) << "Starting simulation with " << num_ticks << " ticks.";
	}

	// FIXME: max_tick_offset of 16 will actually correspond to a max of 15 ticks in the future being able to be specified. We need error checking for this as well.

	// Iterate through each tick
	for (unsigned int tick = 0; tick < num_ticks; tick++) {
		if (tick % report_frequency == 0) {
			std::cout << "Tick " << tick + 1 << " started" << std::endl;
		}

		LOG_INFO_(0) << "Tick " << tick + 1 << ":";

		if (Config::traceSpecified()) {
			LOG_DEBUG_(1) << "-------------------- Tick " << tick + 1 << " begins --------------------";
		}

		for (auto core_iter: cores) {
			core_iter->scheduler->updateCurrentWord();
		}

		// Receive all input spike packets destined for this tick	
		std::vector<Packet*>::iterator packet_iter;
		for (auto packet_iter: input_packets[tick]) {
			cores.front()->router->receiveLocal(*packet_iter);
		}
		
		// Next loop through all cores, simulating a tick. Performs all neuron block operations
		for (auto core_iter: cores) {
			core_iter->token_controller->run();
		}
	}	
}