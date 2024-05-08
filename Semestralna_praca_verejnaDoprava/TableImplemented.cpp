#include "TableImplemented.h"

TableImplemented::~TableImplemented()
{
	for (auto& [key_, data_] : table_)
	{
		delete data_;
		data_ = nullptr;
	}
}

void TableImplemented::addTableItemsToTable(FileHandler<BusStop>& carrier)
{
	ImplicitList<BusStop>** pom = nullptr;
	for (const auto& it : carrier) {
		if (table_.tryFind(it.getCarrierSystem(), pom))
		{
			(*pom)->addBusStop(&it);
		}
		else
		{
			const auto list = new ImplicitList<BusStop>;
			list->addBusStop(&it);
			table_.insert(it.getCarrierSystem(), list);
		}
	}
}
