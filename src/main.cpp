/// main.cpp
/// 
/// Created for the University of Arizona Reconfigurable Computing Lab
///

#include <iostream>
#include <fstream>

#include <cxxopts.hpp>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <rapidjson/document.h>

#include "decode.hpp"
#include "truenorthgrid.h"
#include "csramrow.h"
#include "packet.h"
#include "config.hpp"
#include "core.h"

// Global parameters for simulation
rapidjson::Document Config::parameters;

int main(int argc, char *argv[]) {

    cxxopts::Options options("TrueNorthSimulator", "A software simulation of the TrueNorth architecture.");
    options.add_options()
        ("i,input", "Input file", cxxopts::value<std::string>())
        ("o,output", "Output file", cxxopts::value<std::string>())
        ("c,config", "Config file", cxxopts::value<std::string>())
        ("ticks", "Number of ticks to run simulation for", cxxopts::value<int>())
        ("t,trace", "Trace file", cxxopts::value<std::string>())
        ("r,report_freq", "Report frequency", cxxopts::value<int>()->default_value("1"))
        ("h,help", "Print help");

    options.positional_help("INPUT_FILE_NAME, OUTPUT_FILE_NAME, CONFIGURATION_FILE_NAME, NUM_TICKS");
    options.parse_positional({"input", "output", "config", "ticks"});
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    std::string input_file_name;
    std::string output_file_name;
    int ticks, report_frequency;

    if (result.count("input")) {
        input_file_name = result["input"].as<std::string>();
    } else {
        std::cout << "[ERROR] Input file not specified." << std::endl << std::endl;
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count("output")) {
        std::remove(result["output"].as<std::string>().c_str());
        static plog::RollingFileAppender<plog::MessageOnlyFormatter> rolling_file_appender_output(result["output"].as<std::string>().c_str());
        plog::init<0>(plog::info, &rolling_file_appender_output);
    } else {
        std::cout << "[ERROR] Output file not specified." << std::endl << std::endl;
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count("config")) {
        Config::setParameters(result["config"].as<std::string>());
    } else {
        std::cout << "[ERROR] Config file not specified." << std::endl << std::endl;
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count("ticks")) {
        ticks = result["ticks"].as<int>();
    } else {
        std::cout << "[ERROR] Number of ticks not specified." << std::endl << std::endl;
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count("report_freq")) {
        report_frequency = result["report_freq"].as<int>();
    } else {
        report_frequency = 1;
    }

    if (Config::traceSpecified()) {
        if (result.count("trace")) {
            std::remove(result["trace"].as<std::string>().c_str());
            static plog::RollingFileAppender<plog::MessageOnlyFormatter> rolling_file_appender_trace(result["trace"].as<std::string>().c_str());
            plog::init<1>(plog::debug, &rolling_file_appender_trace);
        } else {
            std::cout << "[ERROR] Trace verbosity is set but no trace file specified." << std::endl << std::endl;
            std::cout << options.help() << std::endl;
            return 0;
        }
    } else if (result.count("trace")) {
        std::cout << "[ERROR] Trace file specified but trace verbosity is set to " << std::to_string(Config::parameters["trace_verbosity"].GetInt()) << "." << std::endl << std::endl;
        std::cout << options.help() << std::endl;
        return 0;
    }

    std::vector<std::vector<Packet*>> input_packets;
    std::vector<Core*> cores;

    try {
        input_packets = Decode::parseInputPackets(input_file_name, ticks);
        cores = Decode::parseCores(input_file_name);
    } catch (const Decode::InputDecodingException& e) {
        std::cout << "[ERROR] Error parsing input: " << e.message << std::endl;
        return 1;
    }

    TrueNorthGrid grid = TrueNorthGrid(input_packets, cores);
    grid.beginActivity(ticks, report_frequency);

    return 0;
}
