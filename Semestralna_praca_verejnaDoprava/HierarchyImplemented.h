#pragma once
#include "BusStop.h"
#include "FileHandler.h"
#include "HierarchyIterator.h"
#include "libds/amt/explicit_hierarchy.h"

class HierarchyImplemented  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	ds::amt::MultiWayEH<NodeData> hierarchy_;

	// The 'createImplicitListFromMunicipality' function creates an 'ImplicitList' from a municipality and returns a pointer to the list.
	[[nodiscard]] static const ImplicitList<const BusStop>* 
		createImplicitListFromMunicipality (FileHandler<BusStop>& carrier, const std::string& municipality);

public:
	~HierarchyImplemented();

	// The 'getHierarchy' function returns a reference to the hierarchy.
	[[nodiscard]] ds::amt::MultiWayEH<NodeData>& getHierarchy() { return hierarchy_; }

	// The 'addSonToParent' function adds a son to a parent node in the hierarchy and returns a reference to the son.
	[[nodiscard]] ds::amt::MWEHBlock<NodeData>& addSonToParent(ds::amt::MWEHBlock<NodeData>& parent,
		const std::string& name,
		const ImplicitList<const BusStop>* implicitList = nullptr);

	// The 'addMunicipalities' function adds municipalities to a carrier node in the hierarchy.
	void addMunicipalities(FileHandler<BusStop>& carrier, ds::amt::MWEHBlock<NodeData>& carrierNode);

	static void printStations(HierarchyIterator it);
};

