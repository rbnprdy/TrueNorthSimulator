/// TrueNorth.h
/// 
/// Created for the University of Arizona Reconfigurable Computing Lab
///
///

#ifndef CORE_H
#define CORE_H

class Router;
class Scheduler;
class TokenController;

#include <string>
#include <vector>

#include "packet.h"
#include "csramrow.h"
#include "neuronblock.h"

class Core{
	public:
		Core();
		Core(Core *north, Core *south, Core *west, Core *east, std::vector<CSRAMRow*> csram, std::vector<int> neuron_instructions, int x, int y);
		
		std::string to_string();

		// Core Components
		Router *router;
		Scheduler *scheduler;
		NeuronBlock *neuron_block;
		std::vector<CSRAMRow*> csram;
		TokenController *token_controller;
		
		// Core Coordinates
		int x, y;
};

#endif // CORE_H