#include "HeatExchangerData.hpp"

using namespace rapidjson;

void HeatExchangerData::acquireData(const char* json) {

	data.Parse(json);

	Value& timestamp = data["timestamp"];
	Value& value = data["COTemp"];


	log.insert( std::pair<std::string,float>(timestamp.GetString(), value.GetFloat()) );

}

std::map<std::string, float> HeatExchangerData::getFullLog() {

	return log;
}


float HeatExchangerData::getTemp() {

	Value& s = data["COTemp"];

	return s.GetFloat();

}

std::string HeatExchangerData::getTempString() {

	Value& s = data["COTemp"];

	return std::to_string(s.GetFloat());

}
