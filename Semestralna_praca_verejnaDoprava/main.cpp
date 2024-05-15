// Semestralna_praca_verejnaDoprava.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "libds/heap_monitor.h"
#include "PublicTransportSystem.h"

int main()
{
    /*prerobit tak aby som pouzival aj prvu uroven sem.
    *struct s {std::string, BusStop*} v obci a dopravcovi je BusStop* = nullptr, v zastavke je BusStop* = ukazuje na zastavku
    *viaccestna hierarchia z [ds::amt::MultiWayExplicitHierarchy] <s>
    *--------------------------------------------------------------------------------------------------------------------------
    *
	*string je dopravca
	*Table<std::string, IZ<BusStop*>*>*
	*analyzer.h
	*->complexity_analyzer.h->tabinsertanalyzer->zdedit complexity<TREAP<std::string, IZ<BusStop*>*>*>->
	*konstruktor std::string& name, growToSize(TREAP, size_t size)[insert data], executeOperation(TREAP) [robit find]
	*da sa to urobit na template, kedze meni sa len execute operation
	*dokument na random cisla na generaciu random dat.
	*/
    initHeapMonitor();
	
    PublicTransportSystem pts;

    return 0;
}

/*//---first level
 *FileHandlerV2<BusStop> cow;
    cow.readFromFile("cow_busstops.csv");

    FileHandlerV2<BusStop> kam;
    kam.readFromFile("kam_busstops.csv");

    FileHandlerV2<BusStop> nan;
    nan.readFromFile("nan_busstops.csv");

    FileHandlerV2<BusStop> vic;
    vic.readFromFile("vic_busstops.csv");

    FileHandlerV2<BusStop> vly;
    vly.readFromFile("vly_busstops.csv");

    FileHandlerV2<BusStop> whi;
    whi.readFromFile("whi_busstops.csv");

    FileHandlerV2<BusStop> wil;
    wil.readFromFile("wil_busstops.csv");

    FileHandlerV2<BusStop> wkt;
    wkt.readFromFile("wkt_busstops.csv");

    std::unordered_map<size_t, FileHandlerV2<BusStop>> csvMap = {
        {1, cow},
        {2, kam},
        {3, nan},
        {4, vic},
        {5, vly},
        {6, whi},
        {7, wil},
        {8, wkt}
    };

    while (true) {
        size_t carrier = checkers::get_valid_carrier();
        size_t number = checkers::get_valid_predicate();
        std::string prefix = checkers::get_valid_string();

        auto it = csvMap.find(carrier);
        auto predicate = which_predicate::choose_predicate(number, prefix);
        const std::vector<BusStop> filteredStations = algorithm::process<BusStop>(
	        it->second.begin(), it->second.end(), predicate);

    	for (const BusStop& col : filteredStations) {
            std::cout << col;
    	}
    	std::cout << '\n';

        std::cout << "Zadajte 'q' pre ukoncenie programu, alebo lubovolny iny vstup pre pokracovanie: ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "q") {
            break;
        }
    }
    return 0;
 */
