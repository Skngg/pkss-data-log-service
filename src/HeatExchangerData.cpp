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
	const std::string params = "host = " + std::string(DB_HOST) +" dbname = " + std::string(DB_NAME) + " user = " + std::string(DB_USER) + " password = " + std::string(DB_PASSWORD);
	pqxx::connection C(params.c_str());
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
