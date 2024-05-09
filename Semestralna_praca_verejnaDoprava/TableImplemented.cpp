#include "TableImplemented.h"

TableImplemented::~TableImplemented()
{
	auto deleteTableData = [](UniversalTable* table) {
		const auto sortedSTab = dynamic_cast<UsedTable*>(table);
		for (auto& item : *sortedSTab) {
			delete item.data_;
			item.data_ = nullptr;
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

	delete cowTable_;
	delete kamTable_;
	delete nanTable_;
	delete vicTable_;
	delete vlyTable_;
	delete whiTable_;
	delete wilTable_;
	delete wktTable_;
}


void TableImplemented::addTableItemsToTable(FileHandler<BusStop>& carrier,
	UniversalTable& table)
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
