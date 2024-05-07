#include <iostream>
#include "Checkers.h"

size_t checkers::get_valid_carrier(const size_t max) {
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

predicate_type checkers::get_valid_predicate() {
    int number;
    std::cout << "Zadajte predikat: " << '\n'
        << "1 - startsWithStr" << '\n' << "2 - containsStr"
        << '\n';

    while (true) {
        std::cin >> number;
        if (number == 1)
            return predicate_type::starts_with;
        if (number == 2)
	        return predicate_type::contains;

        std::cin.clear(); // clear the error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the rest of the line
        std::cerr << "Neplatny predikat! Skuste znova." << '\n';
    }
}

std::string checkers::get_valid_string() {
    std::string prefix;
    std::cout << "Zadajte retazec:" << '\n';
    std::cin >> std::ws; // Skip leading whitespace

    while (true) {
        std::getline(std::cin, prefix);

        if (!prefix.empty()) {
            return prefix;
        }

        std::cerr << "Prazdny retazec, skuste znova!" << '\n';
    }
}