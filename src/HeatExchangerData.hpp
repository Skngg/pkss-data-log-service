#ifndef HEATEXCHANGERDATA_HPP_
#define HEATEXCHANGERDATA_HPP_

#include "JSONData.hpp"
#include <map>

class HeatExchangerData: public JSONData {
private:
	std::map<std::string,float> log;
public:
	void acquireData(const char* json);
	float getTemp();
	std::string getTempString();
	std::map<std::string,float> getFullLog();
};

#endif /* HEATEXCHANGERDATA_HPP_ */
