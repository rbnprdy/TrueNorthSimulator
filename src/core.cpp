/// core.cpp
/// 
/// Created for the University of Arizona Reconfigurable Computing Lab
///
///

#include "core.h"
#include "config.hpp"
#include "router.h"
#include "scheduler.h"
#include "tokencontroller.h"

Core::Core() {
	this->router = new Router(this, NULL, NULL, NULL, NULL);
	this->scheduler = new Scheduler(this);
	this->neuron_block = new NeuronBlock();
	this->csram = std::vector<CSRAMRow*>(Config::parameters["num_neurons"].GetInt(), new CSRAMRow());
	this->token_controller = new TokenController(this, router, scheduler, neuron_block, csram, std::vector<int>(Config::parameters["num_axons"].GetInt()));
	this->x = 0;
	this->y = 0;
}

Core::Core(Core *north, Core *south, Core *west, Core *east, std::vector<CSRAMRow*> csram, std::vector<int> neuron_instructions, int x, int y){
	this->router = new Router(this, north != NULL ? north->router : NULL, south != NULL ? south->router : NULL, west != NULL ? west->router : NULL, east != NULL ? east->router : NULL);
	this->scheduler = new Scheduler(this);
	this->neuron_block = new NeuronBlock();
	this->csram = csram;
	this->token_controller = new TokenController(this, router, scheduler, neuron_block, csram, neuron_instructions); 
	this->x = x;
	this->y = y;
}

std::string Core::to_string() {
	return "coordinates: (" + std::to_string(this->x) + "," + std::to_string(this->y) + ")";
}