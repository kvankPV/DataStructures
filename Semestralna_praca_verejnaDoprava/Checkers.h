#pragma once
#include <string>
#include "PredicateType.h"

/* The namespace 'checkers' is used to encapsulate the functions
'get_valid_carrier', 'get_valid_predicate', and 'get_valid_string'.*/
namespace checkers
{
    /* The function 'get_valid_carrier' takes a maximum value and prompts the user to enter a number.
    It validates the input and ensures that the entered number is less than or equal to the maximum value.*/
    size_t GetValidCarrier(size_t max);

    /*The function 'get_valid_predicate' prompts the user to choose a predicate type.
	It validates the input and returns the chosen predicate type. */
    PREDICATE_TYPE GetValidPredicate();

    /* The function 'get_valid_string' prompts the user to enter a string.
    It validates the input and ensures that the entered string is not empty. */
    std::string GetValidString();
}