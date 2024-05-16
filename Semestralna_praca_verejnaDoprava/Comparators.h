#pragma once

#include "libds/adt/sorts.h"
#include <algorithm>

namespace Comparators {
    template <typename T>
    void sortAlphabetically(ds::amt::ImplicitSequence<T*>& is) {
        ds::adt::MergeSort<T*> mergeSort;
        mergeSort.sort(is, [](const T* a, const T* b) {
            return a->getStopName() < b->getStopName();
            });
    }

    template <typename T>
    void sortByConsonants(ds::amt::ImplicitSequence<T*>& is) {
        ds::adt::MergeSort<T*> mergeSort;
        mergeSort.sort(is, [](const T* a, const T* b) {
            auto consonantCount = [](const std::string& s) {
                return std::count_if(s.begin(), s.end(), [](unsigned char c) {
                    return std::string("bc�d�fghjkl��mn�pqr�s�t�vwxz�").find(tolower(c)) != std::string::npos;
                    });
                };
            return consonantCount(a->getStopName()) < consonantCount(b->getStopName());
            });
    }
};