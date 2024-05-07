#pragma once
#include <functional>
#include <string>

#include "BusStop.h"
#include "PredicateType.h"

// The namespace 'which_predicate' is used to encapsulate the function 'choose_predicate'
namespace which_predicate
{
    /*The function 'choose_predicate' is a higher-order function that takes a predicate type and a string prefix
    and returns a function that takes a pointer to a BusStop and returns a boolean.
    The returned function checks if the BusStop's name starts with or contains the provided prefix, 
    depending on the predicate type.*/
    std::function<bool(const BusStop*)> choose_predicate
    (predicate_type type, const std::string& prefix);
}