// Semestralna_praca_verejnaDoprava.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "libds/heap_monitor.h"
#include "PublicTransportSystem.h"


int main()
{
    /*prerobit tak aby som pouzival aj prvu uroven sem.
    *struct s {std::string, BusStop*} v obci a dopravcovi je BusStop* = nullptr, v zastavke je BusStop* = ukazuje na zastavku
    *viaccestna hierarchia z [ds::amt::MultiWayExplicitHierarchy] <s>
    *
	*string je dopravca
	*Table<std::string, IZ<BusStop*>*>*
	*analyzer.h
	*->complexity_analyzer.h->tabinsertanalyzer->zdedit complexity<TREAP<std::string, IZ<BusStop*>*>*>->
	*konstruktor sts::string& name, growToSize(TREAP, size_t size)[insert data], executeOperation(TREAP) [robit find]
	*da sa to urobit na template, kedze meni sa len execute operation
	*dokument na random cisla na generaciu random dat.
	*/
    initHeapMonitor();
    PublicTransportSystem pts;

	auto& root = pts.getHierarchy().emplaceRoot();
	root.data_.name = "root_";
	root.data_.order = 0;
	root.data_.busStopList = nullptr;

	auto& cowNode = pts.addSonToParent(root,
		"Cowichan Valley Regional Transit System");
	auto& kamNode = pts.addSonToParent(root,
		"Kamloops Transit System");
	auto& nanNode = pts.addSonToParent(root,
		"Regional District of Nanaimo Transit System");
	auto& vicNode = pts.addSonToParent(root,
		"Victoria Regional Transit System");
	auto& vlyNode = pts.addSonToParent(root,
		"Fraser Valley Region");
	auto& whiNode = pts.addSonToParent(root,
		"Whistler Transit System");
	auto& wilNode = pts.addSonToParent(root,
		"Williams Lake Transit System");
	auto& wktNode = pts.addSonToParent(root,
		"West Kootenay Transit System");

	FileHandler<BusStop> cow;
	cow.readFromFile("cow_busstops.csv");
	pts.addMunicipalities(cow, cowNode);

	FileHandler<BusStop> kam;
	kam.readFromFile("kam_busstops.csv");
	pts.addMunicipalities(kam, kamNode);

	FileHandler<BusStop> nan;
	nan.readFromFile("nan_busstops.csv");
	pts.addMunicipalities(nan, nanNode);

	FileHandler<BusStop> vic;
	vic.readFromFile("vic_busstops.csv");
	pts.addMunicipalities(vic, vicNode);

	FileHandler<BusStop> vly;
	vly.readFromFile("vly_busstops.csv");
	pts.addMunicipalities(vly, vlyNode);

	FileHandler<BusStop> whi;
	whi.readFromFile("whi_busstops.csv");
	pts.addMunicipalities(whi, whiNode);

	FileHandler<BusStop> wil;
	wil.readFromFile("wil_busstops.csv");
	pts.addMunicipalities(wil, wilNode);

	FileHandler<BusStop> wkt;
	wkt.readFromFile("wkt_busstops.csv");
	pts.addMunicipalities(wkt, wktNode);

	pts.runCommandLoop();

    return 0;
}
