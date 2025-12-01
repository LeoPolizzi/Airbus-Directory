#pragma once

#include "Person.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <ctime>
#include <simdjson.h>
#include <nlohmann/json.hpp>
#include <exception>
#ifdef _WIN32
    #include <winsock2.h>     // socket(), sendto(), sockaddr_in, closesocket
    #include <ws2tcpip.h>     // inet_pton()
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>   // socket(), sendto()
    #include <arpa/inet.h>    // htons(), inet_pton(), sockaddr_in
    #include <unistd.h>       // close()
#endif

#define ENTRIES_KEY "annuaire"

class Directory {
private:
	std::vector<Person> entries;
	std::string jsonFilename;
	time_t lastModifiedTime;
	std::mutex lock;
	Directory() = delete;

public:
	Directory(const std::string& filename);

	Directory(const Directory& other);

	~Directory();

	Directory& operator=(const Directory& other);

	void listPersons() const;
	void addPerson(const Person& person);
	void modifyPerson(const std::string& firstName, const std::string& lastName, const Person& updatedPerson);
	void removePerson(const std::string& firstName, const std::string& lastName);

	void lockMutex();
	void unlockMutex();

	bool hasExternalModification() const;

	void loadFromJSON(const std::string& filename);
	void saveToJSON() const;

	bool sendUDP(const std::string& ip, const int port) const;
};
