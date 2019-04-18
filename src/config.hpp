/// config.hpp
/// 
/// Created for the University of Arizona Reconfigurable Computing Lab
///

#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <assert.h>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

// Used to create assert statements with error messages
#define m_assert(expr, msg) assert(( (void)(msg), (expr) ))

class ConfigDecodingException : public std::exception {
        
    public:
        const char* message;

        ConfigDecodingException(std::string message) {
            this->message = message.c_str();
        }

        virtual const char* what() const throw () {
            return message;
        }
};

class Config {
    public:
        static rapidjson::Document parameters;

        static void setParameters(std::string file_name) {

            FILE* fp = std::fopen(file_name.c_str(), "r");
            // FIXME: Is this size going to matter?
            char readBuffer[65536];
            rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
            
            if (parameters.ParseStream(is).HasParseError()) {
                throw ConfigDecodingException("Could not parse input JSON");
            }
        }

        static bool traceSpecified() {
            return parameters["neuron_block_trace_verbosity"].GetInt() || parameters["token_controller_trace_verbosity"].GetInt() || parameters["scheduler_trace_verbosity"].GetInt();
        }
    private:
        static void validateIntParameterMinMax(std::string name, int min_val, int max_val) {
            if (!parameters.HasMember(name.c_str())) {
                throw ConfigDecodingException("Configuration file does not have parameter " + name + ".");
            }
            if (!parameters[name.c_str()].IsInt()) {
                throw ConfigDecodingException("Configuration parameter " + name + " could not be parsed as integer.");
            }
            if (parameters[name.c_str()].GetInt() < min_val) {
                throw ConfigDecodingException("Configuration parameter " + name + " must be >= " + std::to_string(min_val));
            }
            if (parameters[name.c_str()].GetInt() > max_val) {
                throw ConfigDecodingException("Configuration parameter " + name + " must be <= " + std::to_string(min_val));
            }
        }

        static void validateIntParameterMin(std::string name, int min_val) {
            if (!parameters.HasMember(name.c_str())) {
                throw ConfigDecodingException("Configuration file does not have parameter " + name + ".");
            }
            if (!parameters[name.c_str()].IsInt()) {
                throw ConfigDecodingException("Configuration parameter " + name + " could not be parsed as integer.");
            }
            if (parameters[name.c_str()].GetInt() < min_val) {
                throw ConfigDecodingException("Configuration parameter " + name + " must be >= " + std::to_string(min_val));
            }
        }

        static void validateIntParameterMax(std::string name, int min_val, int max_val) {
            if (!parameters.HasMember(name.c_str())) {
                throw ConfigDecodingException("Configuration file does not have parameter " + name + ".");
            }
            if (!parameters[name.c_str()].IsInt()) {
                throw ConfigDecodingException("Configuration parameter " + name + " could not be parsed as integer.");
            }
            if (parameters[name.c_str()].GetInt() > max_val) {
                throw ConfigDecodingException("Configuration parameter " + name + " must be <= " + std::to_string(min_val));
            }
        }

        static void validateParameters() {
            validateIntParameterMin("num_neurons", 0);
            validateIntParameterMin("num_axons", 0);
            validateIntParameterMin("num_cores_x", 0);
            validateIntParameterMin("num_cores_y", 0);
            validateIntParameterMin("num_weights", 0);
            validateIntParameterMin("max_tick_offset", 0);
            validateIntParameterMinMax("neuron_block_trace_verbosity", 0, 2);
            validateIntParameterMinMax("token_controller_trace_verbosity", 0, 1);
            validateIntParameterMinMax("scheduler_trace_verbosity", 0, 2);
        }
};

#endif // CONFIG_H
