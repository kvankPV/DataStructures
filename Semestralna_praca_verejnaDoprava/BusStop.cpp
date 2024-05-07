#include "BusStop.h"

BusStop::BusStop(const std::vector<std::string>& data)
{
	if (data.size() != 8)
		throw std::invalid_argument("Invalid data size!");
	stopId_ = data[0];
	//std::copy(data[0].begin(), data[0].end(), stopId_);
	stopName_ = data[1];
	stopSite_ = data[2];
	latitude_ = std::stod(data[3]);
	longitude_ = std::stod(data[4]);
	//std::copy(data[5].begin(), data[5].end(), systemCode_);
	carrierSystem_ = data[6];
	municipality_ = data[7];
}