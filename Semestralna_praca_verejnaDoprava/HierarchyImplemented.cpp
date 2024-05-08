#include "HierarchyImplemented.h"

#include "Algorithm.h"
#include "Checkers.h"
#include "PredicateType.h"
#include "WhichPredicate.h"

/*
 * Responsible for deallocating memory used by the ImplicitList objects in the hierarchy.
 *It uses a recursive function deleteImplicitLists to traverse the hierarchy and delete each ImplicitList.
 */
HierarchyImplemented::~HierarchyImplemented()
{
	// Start from the root of the hierarchy
	const auto root = hierarchy_.accessRoot();

	// Recursive function to delete all ImplicitList* in the hierarchy
	std::function<void(ds::amt::MWEHBlock<NodeData>&)> deleteImplicitLists = [&](ds::amt::MWEHBlock<NodeData>& node) {
		// Delete and nullify the ImplicitList* of the current node
		delete node.data_.busStopList;
		node.data_.busStopList = nullptr;

		// Recurse for each son of the current node
		for (size_t i = 0; i < hierarchy_.degree(node); i++) {
			const auto son = hierarchy_.accessSon(node, i);
			deleteImplicitLists(*son);
		}
	};

	// Call the recursive function starting from the root
	deleteImplicitLists(*root);
}

/*
 *Adds a new node to the hierarchy as a child of the specified parent node.
 *The new node is initialized with the provided name and ImplicitList.
 *If no ImplicitList is provided, the busStopList of the new node is set to nullptr.
 */
[[nodiscard]] ds::amt::MWEHBlock<NodeData>& HierarchyImplemented::addSonToParent(ds::amt::MWEHBlock<NodeData>& parent,
	const std::string& name,
	ImplicitList<const BusStop>* implicitList)
{
	auto& son = hierarchy_.emplaceSon(parent, parent.sons_->size());
	son.data_.order = parent.sons_->size() - 1;
	son.data_.name = name;
	son.data_.busStopList = implicitList;

	return son;
}

/*
 *This function iterates over a FileHandler of BusStop objects and adds a new node to the hierarchy for each unique municipality found.
 *Each new node is initialized with an ImplicitList created from the BusStop objects of the corresponding municipality.
 */
void HierarchyImplemented::addMunicipalities(FileHandler<BusStop>& carrier, ds::amt::MWEHBlock<NodeData>& carrierNode)
{
	for (const auto& it : carrier) {

		const std::string& municipality = it.getMunicipality();
		bool isDuplicate = false;

		for (size_t i = 0; i < hierarchy_.degree(carrierNode); i++) {
			if (const auto son = hierarchy_.accessSon(carrierNode, i);
				son->data_.name == municipality) {
				isDuplicate = true;
				break;
			}
		}

		if (!isDuplicate) {
			ImplicitList<const BusStop>* implicitList = createImplicitListFromMunicipality(carrier, municipality);
			auto& son = hierarchy_.emplaceSon(carrierNode,
				carrierNode.sons_->size());
			son.data_.order = carrierNode.sons_->size() - 1;
			son.data_.name = municipality;
			if (implicitList != nullptr) {
				son.data_.busStopList = implicitList;
			}
		}
	}
}

/*
 *Creates a new ImplicitList from the BusStop objects of a specified municipality.
 *It iterates over a FileHandler of BusStop objects and adds each BusStop of the specified municipality to the new ImplicitList.
 */
[[nodiscard]] ImplicitList<const BusStop>* HierarchyImplemented::createImplicitListFromMunicipality(FileHandler<BusStop>& carrier,
	const std::string& municipality)
{
	const auto implicitList = new ImplicitList<const BusStop>;
	for (const BusStop& busStop : carrier)
	{
		if (busStop.getMunicipality() == municipality)
		{
			implicitList->addBusStop(&busStop);
		}
	}
	return implicitList;
}

void HierarchyImplemented::printStations(HierarchyIterator it)
{
	const PREDICATE_TYPE predicateType = checkers::GetValidPredicate();
	const std::string prefix = checkers::GetValidString();
	if (auto predicate = which_predicate::ChoosePredicate<const BusStop>(predicateType, prefix,
	                                                                           [](const BusStop* busStop) { return busStop->getStopName(); })) {
		for (auto& [order, name, busStopList] : it)
		{
			if (busStopList != nullptr) {
				algorithm::Process<ImplicitList<BusStop>::iterator, const BusStop>
					(busStopList->begin(), busStopList->end(), predicate, [](const BusStop* busStop)
						{
							std::cout << *busStop << '\n';
						}
				);
			}
		}
	}
}