#pragma once
#include <string>
#include <vector>
#include <ostream>

class BusStop
{
private:
	std::string stopId_;
	std::string stopName_;
	std::string stopSite_;
	double latitude_ = 0;
	double longitude_ = 0;
	std::string systemCode_;
	std::string carrierSystem_;
	std::string municipality_;

public:
	explicit BusStop(const std::vector<std::string>& data);

	//Marked as 'nodiscard' to indicate that the compiler will warn if the return value is discarded.
	[[nodiscard]] const std::string& getStopId() const { return stopId_; }
	[[nodiscard]] const std::string& getStopName() const { return stopName_; }
	[[nodiscard]] const std::string& getStopSite() const { return stopSite_; }
	[[nodiscard]] double getLatitude() const { return latitude_; }
	[[nodiscard]] double getLongitude() const { return longitude_; }
	[[nodiscard]] const std::string& getSystemCode() const { return systemCode_; }
	[[nodiscard]] const std::string& getCarrierSystem() const { return carrierSystem_; }
	[[nodiscard]] const std::string& getMunicipality() const { return municipality_; }

	// Overloaded '<<' operator to print the properties of the bus stop to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const BusStop& busStop)
	{
		os << "Stop ID: " << busStop.getStopId() << '\n'
			<< "Stop Name: " << busStop.getStopName() << '\n'
			<< "Stop Site: " << busStop.getStopSite() << '\n'
			<< "Latitude: " << busStop.getLatitude() << '\n'
			<< "Longitude: " << busStop.getLongitude() << '\n'
			<< "System code: " << busStop.getSystemCode() << '\n'
			<< "Carrier System: " << busStop.getCarrierSystem() << '\n'
			<< "Municipality: " << busStop.getMunicipality() << '\n'
			<< '\n';
		return os;
	}
};