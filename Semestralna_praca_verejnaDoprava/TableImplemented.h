#pragma once
#include "BusStop.h"
#include "FileHandler.h"
#include "ImplicitList.h"
#include "libds/adt/table.h"

class TableImplemented  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> cowTable_;
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> kamTable_;
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> nanTable_;
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> vicTable_;
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> vlyTable_;
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> whiTable_;
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> wilTable_;
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*> wktTable_;

public:
	~TableImplemented();
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getCowTable() { return cowTable_; }
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getKamTable() { return kamTable_; }
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getNanTable() { return nanTable_; }
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getVicTable() { return vicTable_; }
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getVlyTable() { return vlyTable_; }
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getWhiTable() { return whiTable_; }
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getWilTable() { return wilTable_; }
	ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& getWktTable() { return wktTable_; }

	static void addTableItemsToTable(FileHandler<BusStop>& carrier,
	                                 ds::adt::SortedSTab<std::string, ImplicitList<BusStop>*>& table);
};

