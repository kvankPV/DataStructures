#pragma once
#include <functional>

namespace algorithm {

    /*The function 'process' is a template function that takes a range of elements (specified by 'begin' and 'end' iterators)
    and a predicate function. It returns a vector of elements from the input range that satisfy the predicate.*/
    template <typename Iterator, typename Container>
    void Process(
        Iterator begin,
        Iterator end,
        std::function<bool(const Container*)>& predicate,
        const std::function<void(const Container*)>& operation = 0);

    template <typename Iterator, typename Container>
    void Process(Iterator begin, Iterator end, std::function<bool(const Container*)>& predicate,
	    const std::function<void(const Container*)>& operation)
    {
        for (Iterator it = begin; it != end; ++it) {
            if (predicate(*it)) {
                operation(*it);
            }
        }
    }
}