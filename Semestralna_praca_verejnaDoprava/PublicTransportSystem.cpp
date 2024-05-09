#include "PublicTransportSystem.h"

#include "Checkers.h"
#include "HierarchyImplemented.h"

PublicTransportSystem::PublicTransportSystem()
{
	auto& root = hi_.getHierarchy().emplaceRoot();
	root.data_.name = "root_";
	root.data_.order = 0;
	root.data_.busStopList = nullptr;

	auto& cowNode = hi_.addSonToParent(root,
		"Cowichan Valley Regional Transit System");
	auto& kamNode = hi_.addSonToParent(root,
		"Kamloops Transit System");
	auto& nanNode = hi_.addSonToParent(root,
		"Regional District of Nanaimo Transit System");
	auto& vicNode = hi_.addSonToParent(root,
		"Victoria Regional Transit System");
	auto& vlyNode = hi_.addSonToParent(root,
		"Fraser Valley Region");
	auto& whiNode = hi_.addSonToParent(root,
		"Whistler Transit System");
	auto& wilNode = hi_.addSonToParent(root,
		"Williams Lake Transit System");
	auto& wktNode = hi_.addSonToParent(root,
		"West Kootenay Transit System");

	FileHandler<BusStop> cow;
	cow.readFromFile("cow_busstops.csv");
	hi_.addMunicipalities(cow, cowNode);
	TableImplemented::addTableItemsToTable(cow, ti_.getCowTable());

	FileHandler<BusStop> kam;
	kam.readFromFile("kam_busstops.csv");
	hi_.addMunicipalities(kam, kamNode);
	TableImplemented::addTableItemsToTable(kam, ti_.getKamTable());

	FileHandler<BusStop> nan;
	nan.readFromFile("nan_busstops.csv");
	hi_.addMunicipalities(nan, nanNode);
	TableImplemented::addTableItemsToTable(nan, ti_.getNanTable());

	FileHandler<BusStop> vic;
	vic.readFromFile("vic_busstops.csv");
	hi_.addMunicipalities(vic, vicNode);
	TableImplemented::addTableItemsToTable(vic, ti_.getVicTable());

	FileHandler<BusStop> vly;
	vly.readFromFile("vly_busstops.csv");
	hi_.addMunicipalities(vly, vlyNode);
	TableImplemented::addTableItemsToTable(vly, ti_.getVlyTable());

	FileHandler<BusStop> whi;
	whi.readFromFile("whi_busstops.csv");
	hi_.addMunicipalities(whi, whiNode);
	TableImplemented::addTableItemsToTable(whi, ti_.getWhiTable());

	FileHandler<BusStop> wil;
	wil.readFromFile("wil_busstops.csv");
	hi_.addMunicipalities(wil, wilNode);
	TableImplemented::addTableItemsToTable(wil, ti_.getWilTable());

	FileHandler<BusStop> wkt;
	wkt.readFromFile("wkt_busstops.csv");
	hi_.addMunicipalities(wkt, wktNode);
	TableImplemented::addTableItemsToTable(wkt, ti_.getWktTable());

	this->runCommandLoop();
}


/*
 *Runs a command loop that allows the user to navigate the hierarchy and perform operations on the BusStop objects.
 *The user can move up and down the hierarchy, print the BusStop objects that satisfy a specified predicate, or quit the command loop.
 */
void PublicTransportSystem::runCommandLoop()
{
	size_t number;
	while (true) {
		std::cout << '\n' << "Select level: " << '\n'
			<< "0 - exit" << '\n'
			<< "2 - second level" << '\n' << "3 - third level" << '\n'
			<< "Input: ";

		std::cin >> number;
		if (number == 2)
		{
			secondLevel();

		} else if (number == 3)
		{
			thirdLevel();

		} else if (number == 0)
		{
			break;

		} else
		{
			std::cin.clear(); // clear the error flags
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the rest of the line
			std::cerr << "Chybny input." << '\n';
		}
	}
}

void PublicTransportSystem::secondLevel()
{
	HierarchyIterator it(&hi_.getHierarchy(), hi_.getHierarchy().accessRoot());

	while (true) {
		std::cout << '\n' << "Current location: " << it->name << '\n';
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
			const size_t sonIndex = checkers::GetValidCarrier(actualSize);
			it.goDown(sonIndex - 1);
		}

		else if (command == 'p') {
			HierarchyImplemented::printStations(it);
		}

		else if (command == 'q') {
			break;
		}
	}
}

void PublicTransportSystem::thirdLevel()
{
	std::string carrier;
	std::cout << "Write the whole name of the carrier: " << '\n'
	<< "	Cowichan Valley Regional Transit System" << '\n'
	<< "	Kamloops Transit System" << '\n'
	<< "	Regional District of Nanaimo Transit System" << '\n'
	<< "	Victoria Regional Transit System" << '\n'
	<< "	Fraser Valley Region" << '\n'
	<< "	Whistler Transit System" << '\n'
	<< "	Williams Lake Transit System" << '\n'
	<< "	West Kootenay Transit System" << '\n';
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
	std::getline(std::cin, carrier);

	// Remove trailing spaces
	carrier.erase(std::find_if(carrier.rbegin(), carrier.rend(), [](const unsigned char ch) {
		return !std::isspace(ch);
		}).base(), carrier.end());

	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>* selectedTable;

	if (carrier == "Cowichan Valley Regional Transit System") {
		selectedTable = &ti_.getCowTable();
	}
	else if (carrier == "Kamloops Transit System") {
		selectedTable = &ti_.getKamTable();
	}
	else if (carrier == "Regional District of Nanaimo Transit System") {
		selectedTable = &ti_.getNanTable();
	}
	else if (carrier == "Victoria Regional Transit System") {
		selectedTable = &ti_.getVicTable();
	}
	else if (carrier == "Fraser Valley Region") {
		selectedTable = &ti_.getVlyTable();
	}
	else if (carrier == "Whistler Transit System") {
		selectedTable = &ti_.getWhiTable();
	}
	else if (carrier == "Williams Lake Transit System") {
		selectedTable = &ti_.getWilTable();
	}
	else if (carrier == "West Kootenay Transit System") {
		selectedTable = &ti_.getWktTable();
	}
	else {
		std::cout << "Carrier not available!" << '\n';
		return;
	}

	for (auto& [key_, data_] : *selectedTable)
	{
		std::cout << key_ << '\n';
	}

	std::string stopName;
	std::cout << "Write the whole name of the stop name: ";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
	std::getline(std::cin, stopName);

	// Remove trailing spaces
	stopName.erase(std::find_if(stopName.rbegin(), stopName.rend(), [](const unsigned char ch) {
		return !std::isspace(ch);
		}).base(), stopName.end());

	if (ImplicitList<BusStop>** list = nullptr; selectedTable->tryFind(stopName, list))
	{
		if (*list != nullptr) {
			for (const BusStop* busStop : **list) {
				std::cout << *busStop << '\n';
			}
		}
	}
	else
	{
		std::cout << "Stop name not available!" << '\n';
	}
}
