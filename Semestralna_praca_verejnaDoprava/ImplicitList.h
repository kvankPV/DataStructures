#pragma once
#include "BusStop.h"
#include "libds/adt/list.h"

class ImplicitList  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	ds::adt::ImplicitList<const BusStop*>* list_;
    using iterator = ds::adt::GeneralList<const BusStop*, ds::amt::IS<const BusStop*>>::IteratorType;

public:
	ImplicitList() : list_(new ds::adt::ImplicitList<const BusStop*>()) {}
    ~ImplicitList() { delete list_; }

    [[nodiscard]] iterator begin() const { return list_->begin(); }
    [[nodiscard]] iterator end() const { return list_->end(); }

    void addBusStop(const BusStop* busStop) const { list_->insertLast(busStop); }
};