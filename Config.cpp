#include "Config.hpp"

#include <simdjson.h>

Config::Config(const std::string& filename) {
    simdjson::dom::parser parser;
    simdjson::dom::element doc;

    auto error = parser.load(filename).get(doc);
    if (error) {
        throw std::runtime_error("Failed to load config file.");
    }

    auto dir = doc[DIRECTORYPATH_KEY];
    auto dip = doc[DESTINATIONIP_KEY];
    auto prt = doc[PORT_KEY];
    auto itv = doc[INTERVAL_KEY];

    if (dir.error() || dip.error() || prt.error() || itv.error()) {
        throw std::runtime_error("Missing config keys.");
    }

    this->directoryPath = std::string(dir.get_string().value());

	if (this->directoryPath.size() < 5 || this->directoryPath.substr(this->directoryPath.size() - 5) != ".json") {
		throw std::invalid_argument("Directory path must be a .json file.");
	}

    this->destinationIP = std::string(dip.get_string().value());
    this->port = int(prt.get_uint64().value());
    this->interval = int(itv.get_uint64().value());
}
