#include "TableImplemented.h"

TableImplemented::~TableImplemented()
{
	auto deleteTableData = [](ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& table) {
		for (auto& [key, data] : table) {
			delete data;
			data = nullptr;
		}
		};

	deleteTableData(cowTable_);
	deleteTableData(kamTable_);
	deleteTableData(nanTable_);
	deleteTableData(vicTable_);
	deleteTableData(vlyTable_);
	deleteTableData(whiTable_);
	deleteTableData(wilTable_);
	deleteTableData(wktTable_);
}


void TableImplemented::addTableItemsToTable(FileHandler<BusStop>& carrier,
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& table)
{
	ImplicitList<BusStop>** pom = nullptr;
	for (const auto& it : carrier) {
		if (table.tryFind(it.getStopName(), pom))
		{
			(*pom)->addBusStop(&it);
		}
		else
		{
			const auto list = new ImplicitList<BusStop>;
			list->addBusStop(&it);
			table.insert(it.getStopName(), list);
		}
	}
}
