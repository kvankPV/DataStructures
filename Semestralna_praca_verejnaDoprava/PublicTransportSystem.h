#pragma once

#include "HierarchyImplemented.h"
#include "TableImplemented.h"

// The 'PublicTransportSystem' class provides a way to manage a hierarchy of 'NodeData' objects.
class PublicTransportSystem  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	// The 'runCommandLoop' function runs a command loop for navigating the hierarchy and printing bus stops that satisfy a certain predicate.
	void runCommandLoop();

	void secondLevel();

	void thirdLevel();

	HierarchyImplemented hierarchyImplemented_;
	TableImplemented tableImplemented_;

public:
	PublicTransportSystem();
};
