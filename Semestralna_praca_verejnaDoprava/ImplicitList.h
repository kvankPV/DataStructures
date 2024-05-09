#pragma once
#include "libds/adt/list.h"

template <typename Container>
class ImplicitList  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	ds::adt::ImplicitList<const Container*>* list_;

public:
    using iterator = typename ds::adt::GeneralList<const Container*, ds::amt::IS<const Container*>>::IteratorType;

	ImplicitList() : list_(new ds::adt::ImplicitList<const Container*>()) {}
    ~ImplicitList() { delete list_; list_ = nullptr; }

    [[nodiscard]] iterator begin() const { return list_->begin(); }
    [[nodiscard]] iterator end() const { return list_->end(); }

    void addBusStop(const Container* container) const { list_->insertLast(container); }

    size_t getSize() { return list_->size(); }
};