#pragma once

#include <string>
#include "Directory.hpp"

#define DESTINATIONIP_KEY "destination_ip"
#define DIRECTORYPATH_KEY "directory_path"
#define PORT_KEY "port"
#define INTERVAL_KEY "interval"

struct Config {
	std::string directoryPath;
	std::string destinationIP;
	int port;
	int interval;
	Config(const std::string& filename);
};
