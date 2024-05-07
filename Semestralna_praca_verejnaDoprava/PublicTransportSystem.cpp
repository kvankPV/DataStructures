#include "PublicTransportSystem.h"

#include "Checkers.h"
#include "whichPredicate.h"
/*
 * Responsible for deallocating memory used by the ImplicitList objects in the hierarchy.
 *It uses a recursive function deleteImplicitLists to traverse the hierarchy and delete each ImplicitList.
 */
PublicTransportSystem::~PublicTransportSystem()
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
[[nodiscard]] ds::amt::MWEHBlock<NodeData>& PublicTransportSystem::addSonToParent(ds::amt::MWEHBlock<NodeData>& parent,
	const std::string& name,
	const ImplicitList* implicitList)
{
	auto& son = hierarchy_.emplaceSon(parent, parent.sons_->size());
	son.data_.order = parent.sons_->size() - 1;
	son.data_.name = name;
	if (implicitList != nullptr) {
		son.data_.busStopList = implicitList;
	}
	else {
		son.data_.busStopList = nullptr;
	}
	return son;
}

/*
 *This function iterates over a FileHandler of BusStop objects and adds a new node to the hierarchy for each unique municipality found.
 *Each new node is initialized with an ImplicitList created from the BusStop objects of the corresponding municipality.
 */
void PublicTransportSystem::addMunicipalities(FileHandler<BusStop>& carrier, ds::amt::MWEHBlock<NodeData>& carrierNode)
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
			const ImplicitList* implicitList = createImplicitListFromMunicipality(carrier, municipality);
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
[[nodiscard]] const ImplicitList* PublicTransportSystem::createImplicitListFromMunicipality(FileHandler<BusStop>& busStops,
	const std::string& municipality)
{
	const ImplicitList* implicitList = new ImplicitList;
	for (const BusStop& busStop : busStops)
	{
		if (busStop.getMunicipality() == municipality)
		{
			implicitList->addBusStop(&busStop);
		}
	}
	return implicitList;
}

/*
 *Runs a command loop that allows the user to navigate the hierarchy and perform operations on the BusStop objects.
 *The user can move up and down the hierarchy, print the BusStop objects that satisfy a specified predicate, or quit the command loop.
 */
void PublicTransportSystem::runCommandLoop()
{
	HierarchyIterator it(&hierarchy_, hierarchy_.accessRoot());

	while (true) {
		std::cout << "Current location: " << it->name << '\n';
		std::cout << "Enter command (u for up, d for down, p for print, q for quit): ";
		char command;
		std::cin >> command;

		if (command == 'u') {
			it.goUp();
		}
		else if (command == 'd') {
			const size_t actualSize = it.getSize();
			for (size_t i = 0; i < actualSize; ++i) {
				it.goDown(i);
				std::cout << i + 1 << ". " << it->name << '\n';
				it.goUp();
			}
			if (actualSize == 0)
			{
				throw std::out_of_range("Cannot go down from a leaf node!");
			}
			const size_t sonIndex = checkers::get_valid_carrier(actualSize);
			it.goDown(sonIndex - 1);
		}

		else if (command == 'p') {
			printStations(it);
		}

		else if (command == 'q') {
			break;
		}
	}
}

void PublicTransportSystem::printStations(HierarchyIterator it)
{
	const predicate_type predicateType = checkers::get_valid_predicate();
	const std::string prefix = checkers::get_valid_string();
	if (const auto predicate = which_predicate::choose_predicate(predicateType, prefix)) {
		std::vector<const BusStop*> filteredBusStops;
		// Function to iterate over the ImplicitList* of a node and apply the predicate
		auto processNode = [&](const NodeData& node) {
			if (node.busStopList != nullptr) {
				// Iterate over all bus stops in the ImplicitList of the node
				for (const BusStop* busStopPtr : *node.busStopList) {
					if (predicate(busStopPtr)) {
						filteredBusStops.push_back(busStopPtr);
					}
				}
			}
			};
		// Process the current node
		processNode(*it);
		// Iterate over all sons of the current node
		for (auto sonIt = it.begin(); sonIt != it.end(); ++sonIt) {
			processNode(*sonIt);
		}
		if (filteredBusStops.empty())
		{
			std::cerr << "Empty List. Nothing Found." << '\n';
		}
		else {
			for (const auto& busStop : filteredBusStops) {
				std::cout << *busStop << '\n';
			}
		}
	}
}