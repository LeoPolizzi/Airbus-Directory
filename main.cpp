#include "Directory.hpp"
#include "Config.hpp"

std::atomic<bool> g_stop(false);

void read_in(std::string& input) {
    if (std::cin.peek() == '\n') {
        std::cin.ignore(1);
    }
    std::getline(std::cin, input);
}

bool read_in(int64_t& input) {
	if (std::cin >> input) {
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		return true;
	} else {
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		return false;
	}
}

void inputThread(Directory& directory) {
    std::string command;
    while (true) {
        try {
            std::cout << "Enter command (list (l), add (a), modify (m), remove (r), exit (e)): ";
			read_in(command);

            if (command == "list" || command == "l") {
                directory.listPersons();
            }

            else if (command == "add" || command == "a") {
                std::string firstName, lastName;
                int64_t height;

                std::cout << "First Name: ";
				read_in(firstName);

                std::cout << "Last Name: ";
                read_in(lastName);

				std::cout << "Height (in centimeters): ";
				while (!read_in(height)) {
					std::cout << "Invalid height. Please enter a number.\n";
					std::cout << "Height (in centimeters): ";
				}

                Person person(firstName, lastName, height);
                directory.addPerson(person);

                std::cout << firstName << " " << lastName
                          << " added. He's " << height
                          << " centimeters tall." << std::endl;
            }

            else if (command == "modify" || command == "m") {
                std::string firstName, lastName;
                int64_t height;

                std::cout << "First Name of person to modify: ";
				read_in(firstName);

                std::cout << "Last Name of person to modify: ";
				read_in(lastName);

				std::cout << "Height (in centimeters): ";
                while (!read_in(height)) {
                    std::cout << "Invalid height. Please enter a number.\n";
					std::cout << "Height (in centimeters): ";
                }

                Person updatedPerson(firstName, lastName, height);
                directory.modifyPerson(firstName, lastName, updatedPerson);

                std::cout << "Height updated for "
                          << firstName << " " << lastName
                          << ". New height: " << height << "." << std::endl;
            }

            else if (command == "remove" || command == "r") {
                std::string firstName, lastName;

                std::cout << "First Name of person to remove: ";
				read_in(firstName);

                std::cout << "Last Name of person to remove: ";
				read_in(lastName);

                directory.removePerson(firstName, lastName);

                std::cout << firstName << " " << lastName
                          << " removed." << std::endl;
            }

            else if (command == "exit" || command == "e") {
                std::cout << "Exiting program." << std::endl;
                g_stop = true;
                break;
            }

            else {
                std::cout << "Unknown command." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error processing command: "
                      << e.what() << std::endl;
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
