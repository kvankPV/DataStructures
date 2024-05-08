#pragma once
#include <stdexcept>
#include "libds/amt/explicit_hierarchy.h"
#include "ImplicitList.h"

/* The 'NodeData' struct represents a node in the hierarchy.
Each node contains an order, a name, and a pointer to an 'ImplicitList' of 'BusStop' pointers.*/
struct NodeData
{
    size_t order = 0;
    std::string name;
    const ImplicitList<const BusStop>* busStopList = nullptr;

    bool operator==(const NodeData& other) const {
        return name == other.name && busStopList == other.busStopList;
    }
};

class HierarchyIterator {
private:
	// A pointer to the hierarchy to iterate over.
    ds::amt::MultiWayExplicitHierarchy<NodeData>* hierarchy_;
    // A pointer to the current node in the hierarchy.
    ds::amt::MWEHBlock<NodeData>* current_;

public:
    HierarchyIterator(ds::amt::MultiWayExplicitHierarchy<NodeData>* hierarchy,
        ds::amt::MWEHBlock<NodeData>* current)
        : hierarchy_(hierarchy), current_(current) {}

    ds::amt::MultiWayEH<NodeData>::PreOrderHierarchyIterator begin()
    {
        return { hierarchy_, current_ };
    }

    ds::amt::MultiWayEH<NodeData>::PreOrderHierarchyIterator end()
    {
        return { hierarchy_, nullptr };
    }

    // The 'goUp' function moves the iterator to the parent of the current node.
    void goUp() {
        if (current_) {
	        const auto parent = hierarchy_->accessParent(*current_);
            if (parent == nullptr) {
                throw std::out_of_range("Cannot go up from the root!");
            }
            current_ = parent;
        }
        else {
            throw std::out_of_range("Current node is not set!");
        }
    }

    // The 'goDown' function moves the iterator to a specific child of the current node.
    void goDown(const size_t sonIndex) {
        if (current_) {
            if (sonIndex >= hierarchy_->degree(*current_)) {
                throw std::out_of_range("Invalid son index!");
            }
            const auto son = hierarchy_->accessSon(*current_, sonIndex);
            current_ = son;
        }
        else {
            throw std::out_of_range("Current node is not set!");
        }
    }

    
	// The 'getSize' function returns the number of children of the current node.
	[[nodiscard]] size_t getSize() const
    {
    	return current_->sons_->size();
    }

	// The dereference operator returns a reference to the data of the current node.
    [[nodiscard]] NodeData& operator*() const {
        return current_->data_;
    }

    // The arrow operator returns a pointer to the data of the current node.
    [[nodiscard]] NodeData* operator->() const {
        return &operator*();
    }
};