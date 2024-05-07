#pragma once

#include "FileHandler.h"
#include "HierarchyIterator.h"

// The 'PublicTransportSystem' class provides a way to manage a hierarchy of 'NodeData' objects.
class PublicTransportSystem  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	ds::amt::MultiWayExplicitHierarchy<NodeData> hierarchy_;

	// The 'createImplicitListFromMunicipality' function creates an 'ImplicitList' from a municipality and returns a pointer to the list.
	[[nodiscard]] static const ImplicitList* createImplicitListFromMunicipality
	(FileHandler<BusStop>& busStops, const std::string& municipality);

	static void printStations(HierarchyIterator it);

public:
	~PublicTransportSystem();

	// The 'getHierarchy' function returns a reference to the hierarchy.
	[[nodiscard]] ds::amt::MultiWayExplicitHierarchy<NodeData>& getHierarchy() { return hierarchy_; }

	// The 'addSonToParent' function adds a son to a parent node in the hierarchy and returns a reference to the son.
	[[nodiscard]] ds::amt::MWEHBlock<NodeData>& addSonToParent(ds::amt::MWEHBlock<NodeData>& parent, const std::string& name,
	                                                           const ImplicitList* implicitList = nullptr);

	// The 'addMunicipalities' function adds municipalities to a carrier node in the hierarchy.
	void addMunicipalities(FileHandler<BusStop>& carrier, ds::amt::MWEHBlock<NodeData>& carrierNode);

	// The 'runCommandLoop' function runs a command loop for navigating the hierarchy and printing bus stops that satisfy a certain predicate.
	void runCommandLoop();
};
