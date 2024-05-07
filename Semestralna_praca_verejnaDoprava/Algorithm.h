#pragma once
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace algorithm {

    /*The function 'process' is a template function that takes a range of elements (specified by 'begin' and 'end' iterators) 
    and a predicate function. It returns a vector of elements from the input range that satisfy the predicate.*/
	template <typename Container>
	std::vector<Container> process(
	typename std::vector<Container>::iterator begin,
	typename std::vector<Container>::iterator end,
	std::function<bool(const Container&)> predicate);
};

template <typename Container>
std::vector<Container> algorithm::process(
    typename std::vector<Container>::iterator begin,
    typename std::vector<Container>::iterator end,
    std::function<bool(const Container&)> predicate)
{
    std::vector<Container> result;
    // The 'std::copy_if' algorithm is used to copy the elements that satisfy the predicate into the 'result' vector.
    std::copy_if(begin, end, std::back_inserter(result), predicate);
    return result;
}