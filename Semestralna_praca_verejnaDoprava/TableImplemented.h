#pragma once
#include "BusStop.h"
#include "FileHandler.h"
#include "ImplicitList.h"
#include "libds/adt/table.h"

class TableImplemented  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> table_;

public:
	~TableImplemented();
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getTable() { return table_; }
	void addTableItemsToTable(FileHandler<BusStop>& carrier);
};

