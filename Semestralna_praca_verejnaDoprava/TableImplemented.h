#pragma once
#include "BusStop.h"
#include "FileHandler.h"
#include "ImplicitList.h"
#include "libds/adt/table.h"

class TableImplemented  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	using UniversalTable = ds::adt::Table<std::string, ImplicitList<BusStop>*>;
	using UsedTable = ds::adt::Treap<std::string, ImplicitList<BusStop>*>;

private:
	UniversalTable* cowTable_ = new UsedTable;
	UniversalTable* kamTable_ = new UsedTable;
	UniversalTable* nanTable_ = new UsedTable;
	UniversalTable* vicTable_ = new UsedTable;
	UniversalTable* vlyTable_ = new UsedTable;
	UniversalTable* whiTable_ = new UsedTable;
	UniversalTable* wilTable_ = new UsedTable;
	UniversalTable* wktTable_ = new UsedTable;

public:
	~TableImplemented();
	[[nodiscard]] UniversalTable& getCowTable() const { return *cowTable_; }
	[[nodiscard]] UniversalTable& getKamTable() const { return *kamTable_; }
	[[nodiscard]] UniversalTable& getNanTable() const { return *nanTable_; }
	[[nodiscard]] UniversalTable& getVicTable() const { return *vicTable_; }
	[[nodiscard]] UniversalTable& getVlyTable() const { return *vlyTable_; }
	[[nodiscard]] UniversalTable& getWhiTable() const { return *whiTable_; }
	[[nodiscard]] UniversalTable& getWilTable() const { return *wilTable_; }
	[[nodiscard]] UniversalTable& getWktTable() const { return *wktTable_; }

	static void addTableItemsToTable(FileHandler<BusStop>& carrier,
		ds::adt::Table<std::string, ImplicitList<BusStop>*>& table);
};

