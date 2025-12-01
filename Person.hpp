#pragma once

#include <string>
#include <simdjson.h>

#define FIRSTNAME_KEY "prenom"
#define LASTNAME_KEY "nom"
#define HEIGHT_KEY "taille"

class Person {
private:
	std::string firstName;
	std::string lastName;
	int64_t height; // in centimeters
	Person() = delete;

public:
	Person(const std::string& firstName, const std::string& lastName, const int64_t height);
	Person(simdjson::dom::object obj);

	Person(const Person& other);

	~Person();

	Person& operator=(const Person& other);

	std::string getFirstName() const;
	void setFirstName(const std::string& firstName);

	std::string getLastName() const;
	void setLastName(const std::string& lastName);

	int64_t getHeight() const;
	void setHeight(const int64_t height);
};
