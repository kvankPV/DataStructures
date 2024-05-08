#include <iostream>
#include "Checkers.h"

size_t checkers::GetValidCarrier(const size_t max) {
    std::cout << "Input: ";
    size_t carrier;
    while (true) {
        if (std::cin >> carrier && carrier <= max)
        {
            return carrier;
        }
        std::cin.clear(); // clear the error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the rest of the line
        std::cerr << "Neplatne cislo! Skuste znova." << '\n';
    }
}

PREDICATE_TYPE checkers::GetValidPredicate() {
    int number;
    std::cout << "Zadajte predikat: " << '\n'
        << "1 - startsWithStr" << '\n' << "2 - containsStr" << '\n'
		<< "Input: ";

    while (true) {
        std::cin >> number;
        if (number == 1)
            return PREDICATE_TYPE::StartsWith;
        if (number == 2)
	        return PREDICATE_TYPE::Contains;

        std::cin.clear(); // clear the error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the rest of the line
        std::cerr << "Neplatny predikat! Skuste znova." << '\n';
    }
}

std::string checkers::GetValidString() {
    std::string prefix;
    std::cout << "Zadajte retazec: ";
    std::cin >> std::ws; // Skip leading whitespace

    while (true) {
        std::getline(std::cin, prefix);

        if (!prefix.empty()) {
            return prefix;
        }

        std::cerr << "Prazdny retazec, skuste znova!" << '\n';
    }
}