/// TrueNorthGrid.h
/// 
/// Created for the University of Arizona Reconfigurable Computing Lab
///
///

#ifndef TRUENORTHGRID_H
#define TRUENORTHGRID_H

#include <vector>
#include <string>

#include "core.h"
#include "packet.h"

class TrueNorthGrid{
	public:
		TrueNorthGrid(std::vector<std::vector<Packet*>> input_packets, std::vector<Core*> cores);

		void beginActivity(int num_ticks, int report_frequency);
	private:
		std::vector<std::vector<Packet*>> input_packets; 
		std::vector<Core*> cores;		
};

#endif
