#include "whichPredicate.h"

// Implementation of the 'choose_predicate' function.
std::function<bool(const BusStop*)> which_predicate::choose_predicate
(const predicate_type type, const std::string& prefix)
{
    std::function<bool(const BusStop*)> predicate;
    switch (type) {
    	/* If the predicate type is 'starts_with',
    	the returned function checks if the BusStop's name starts with the prefix. */
    case predicate_type::starts_with:
        predicate = [prefix](const BusStop* busStop) {
            return busStop->getStopName().find(prefix) == 0;
            };
        break;
        /* If the predicate type is 'contains',
        the returned function checks if the BusStop's name contains the prefix.*/
    case predicate_type::contains:
        predicate = [prefix](const BusStop* busStop) {
            return busStop->getStopName().find(prefix) != std::string::npos;
            };
        break;
    }
    return predicate;
}