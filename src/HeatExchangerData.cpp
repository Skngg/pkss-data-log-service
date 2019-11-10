#include "HeatExchangerData.hpp"

using namespace rapidjson;

unsigned int HeatExchangerData::dbID(1);

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


void HeatExchangerData::insertLastIntoDB() {
	pqxx::connection C("host = localhost dbname = pkssdatalog user = pkssservice password = kolek");;
	pqxx::work W(C);

	std::string timestamp = (--log.end())->first, status = "running";
	float temperature = log.end()->second;
	unsigned int id = dbID++;
	log.erase(timestamp);
	W.exec0(
			"INSERT INTO exchanger "
			"VALUES (" + W.quote(id) + ", " +
			W.quote(status) + ", " +
			W.quote(timestamp) + ")"
	);

	W.commit();
}
