#pragma once
#include <functional>
#include <string>

#include "PredicateType.h"

// The namespace 'which_predicate' is used to encapsulate the function 'choose_predicate'
namespace which_predicate
{
    /*The function 'choose_predicate' is a higher-order function that takes a predicate type and a string prefix
    and returns a function that takes a pointer to a BusStop and returns a boolean.
    The returned function checks if the BusStop's name starts with or contains the provided prefix, 
    depending on the predicate type.*/
    template <typename Container>
    std::function<bool(const Container*)> ChoosePredicate
    (const PREDICATE_TYPE type, const std::string& prefix)
    {
        std::function<bool(const Container*)> predicate;
        switch (type) {
        case PREDICATE_TYPE::StartsWith:
            predicate = [prefix](const Container* container) {
                return container->getStopName().find(prefix) == 0;
                };
            break;
        case PREDICATE_TYPE::Contains:
            predicate = [prefix](const Container* container) {
                return container->getStopName().find(prefix) != std::string::npos;
                };
            break;
        }
        return predicate;
    }
}