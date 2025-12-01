#include "Directory.hpp"
#include "Config.hpp"

std::atomic<bool> g_stop(false);

void inputThread(Directory& directory) {
	std::string command;
	while (true) {
		try {
			std::cout << "Enter command (list, add, modify, remove, exit): ";
			std::cin >> command;
			if (command == "list") {
				directory.lockMutex();
				directory.listPersons();
				directory.unlockMutex();
			} else
			if (command == "add") {
				std::string firstName, lastName;
				int64_t height;
				std::cout << "First Name: ";
				std::cin >> firstName;
				std::cout << "Last Name: ";
				std::cin >> lastName;
				std::cout << "Height: ";
				std::cin >> height;
				Person person(firstName, lastName, height);
				directory.lockMutex();
				directory.addPerson(person);
				directory.unlockMutex();
				std::cout << firstName << " " << lastName << " added. He's " << height << " centimeters tall." << std::endl;
			} else if (command == "modify") {
				std::string firstName, lastName;
				int64_t height;
				std::cout << "First Name of person to modify: ";
				std::cin >> firstName;
				std::cout << "Last Name of person to modify: ";
				std::cin >> lastName;
				std::cout << "New Height: ";
				std::cin >> height;
				Person updatedPerson(firstName, lastName, height);
				directory.lockMutex();
				directory.modifyPerson(firstName, lastName, updatedPerson);
				directory.unlockMutex();
				std::cout << "Height updated for " << firstName << " " << lastName << "." << " New height: " << height << "." << std::endl;
			} else if (command == "remove") {
				std::string firstName, lastName;
				std::cout << "First Name of person to remove: ";
				std::cin >> firstName;
				std::cout << "Last Name of person to remove: ";
				std::cin >> lastName;
				directory.lockMutex();
				directory.removePerson(firstName, lastName);
				directory.unlockMutex();
				std::cout << firstName << " " << lastName << " removed." << std::endl;
			} else if (command == "exit") {
				std::cout << "Exiting program." << std::endl;
				g_stop = true;
				break;
			} else {
				std::cout << "Unknown command." << std::endl;
			}
		} catch (const std::exception& e) {
			std::cerr << "Error processing command: " << e.what() << std::endl;
			directory.unlockMutex();
		}
	}
}

void monitorThread(Directory& directory, Config& config) {
	while (!g_stop) {
		std::this_thread::sleep_for(std::chrono::seconds(config.interval));
		try {
			directory.lockMutex();
			if (directory.hasExternalModification()) {
				directory.loadFromJSON(config.directoryPath);
				directory.sendUDP(config.destinationIP, config.port);
			}
			directory.unlockMutex();
		} catch (const std::exception& e) {
			std::cerr << "Error in monitor thread: " << e.what() << std::endl;
			directory.unlockMutex();
		}
	}
}

int main(int ac, char**av) {
	if (ac != 2) {
		std::cerr << "Usage: " << av[0] << " <config.json>" << std::endl;
		return 1;
	}
	try {
		Config config(av[1]);
		Directory directory(config.directoryPath);

		std::thread t1(inputThread, std::ref(directory));
		std::thread t2(monitorThread, std::ref(directory), std::ref(config));

		t1.join();
		t2.join();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
