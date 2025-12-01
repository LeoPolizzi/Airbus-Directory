#include "Person.hpp"

Person::Person(const std::string& firstName, const std::string& lastName, const int64_t height)
: firstName(firstName), lastName(lastName), height(height) {
	if (height < 0) {
		throw std::invalid_argument("Height cannot be negative");
	}
}

Person::Person(simdjson::dom::object obj) {
	firstName = std::string(obj[FIRSTNAME_KEY]);
	lastName = std::string(obj[LASTNAME_KEY]);
	height = int64_t(obj[HEIGHT_KEY]);
	if (height < 0) {
		throw std::invalid_argument("Height cannot be negative");
	}
}

Person::Person(const Person& other)
: firstName(other.firstName), lastName(other.lastName), height(other.height) {}

Person::~Person() {}

Person& Person::operator=(const Person& other) {
	if (this != &other) {
		firstName = other.firstName;
		lastName = other.lastName;
		height = other.height;
	}
	return *this;
}

std::string Person::getFirstName() const {
	return firstName;
}

std::string Person::getLastName() const {
	return lastName;
}

int64_t Person::getHeight() const {
	return height;
}

void Person::setFirstName(const std::string& firstName) {
	this->firstName = firstName;
}

void Person::setLastName(const std::string& lastName) {
	this->lastName = lastName;
}

void Person::setHeight(const int64_t height) {
	if (height < 0) {
		throw std::invalid_argument("Height cannot be negative");
	} else {
	this->height = height;
	}
}
