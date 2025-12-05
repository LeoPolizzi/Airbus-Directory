#include "Directory.hpp"

Directory::Directory(const std::string& filename) : jsonFilename(filename) {
	loadFromJSON(filename);
}

Directory::Directory(const Directory& other) {
	this->entries = other.entries;
	this->jsonFilename = other.jsonFilename;
	this->lastModifiedTime = other.lastModifiedTime;
}

Directory::~Directory() {}

Directory& Directory::operator=(const Directory& other) {
	if (this != &other) {
		this->entries = other.entries;
		this->jsonFilename = other.jsonFilename;
		this->lastModifiedTime = other.lastModifiedTime;
	}
	return *this;
}

void Directory::listPersons() {
	lockMutex();
	for (const auto& person : this->entries) {
		std::cout << person.getFirstName() << " " << person.getLastName() << ", Height: " << person.getHeight() << " cm" << std::endl;
	}
	unlockMutex();
}

void Directory::addPerson(const Person& person) {
	lockMutex();
	this->entries.push_back(person);
	saveToJSON();
	unlockMutex();
}

void Directory::modifyPerson(const std::string& firstName, const std::string& lastName, const Person& updatedPerson) {
	lockMutex();
	for (auto& person : this->entries) {
		if (person.getFirstName() == firstName && person.getLastName() == lastName) {
			person = updatedPerson;
			break;
		}
	}
	saveToJSON();
	unlockMutex();
}

void Directory::removePerson(const std::string& firstName, const std::string& lastName) {
	lockMutex();
	this->entries.erase(std::remove_if(this->entries.begin(), this->entries.end(),
		[&](const Person& p) {
			return p.getFirstName() == firstName && p.getLastName() == lastName;
		}), this->entries.end());
	try {
		saveToJSON();
	} catch (const std::exception& e) {
		std::cerr << "Error saving to JSON after removal: " << e.what() << std::endl;
	}
	unlockMutex();
}

void Directory::lockMutex() {
	lock.lock();
}

void Directory::unlockMutex() {
	lock.unlock();
}

bool Directory::hasExternalModification() const {
	auto currentModifiedTime = std::filesystem::last_write_time(this->jsonFilename).time_since_epoch().count();
	return currentModifiedTime != this->lastModifiedTime;
}

void Directory::saveToJSON() const {
	nlohmann::json j;
	j[ENTRIES_KEY] = nlohmann::json::array();
	for (const auto& person : this->entries) {
		nlohmann::json personJson;
		personJson[FIRSTNAME_KEY] = person.getFirstName();
		personJson[LASTNAME_KEY] = person.getLastName();
		personJson[HEIGHT_KEY] = person.getHeight();
		j[ENTRIES_KEY].push_back(personJson);
	}
	std::ofstream file(this->jsonFilename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file for writing: " + this->jsonFilename);
	}
	file << j.dump(4);
	file.close();
}

void Directory::loadFromJSON(const std::string& filename) {
	this->jsonFilename = filename;
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file: " + filename);
	}
	std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	simdjson::dom::parser parser;
	simdjson::dom::element element = parser.parse(jsonContent);
	simdjson::dom::array jsonArray = element[ENTRIES_KEY];
	this->entries.clear();
	for (const auto& item : jsonArray) {
		Person person(item.get<simdjson::dom::object>());
		this->entries.push_back(person);
	}
	this->lastModifiedTime = std::filesystem::last_write_time(this->jsonFilename).time_since_epoch().count();
}

#ifdef _WIN32

void Directory::sendUDP(const std::string& ip, int port) const {
    std::ifstream file(this->jsonFilename);
    if (!file.is_open()) {
		throw std::runtime_error("Could not open file: " + this->jsonFilename);
    }
    std::string data((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    file.close();

	Socket sock(ip, port);

	sock.send(data);
}

#else

void Directory::sendUDP(const std::string& ip, int port) const {
    std::ifstream file(this->jsonFilename);
    if (!file.is_open()) {
		throw std::runtime_error("Could not open file: " + this->jsonFilename);
    }
    std::string data((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    file.close();

	Socket sock(ip, port);

	sock.send(data);
}

#endif
