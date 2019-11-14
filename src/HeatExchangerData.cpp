#include "HeatExchangerData.hpp"
#include<iostream>

using namespace rapidjson;

//unsigned int HeatExchangerData::dbID(1);
const std::string params =	"host = " + std::string(DB_HOST) +
							" dbname = " + std::string(DB_NAME) +
							" user = " + std::string(DB_USER) +
							" password = " + std::string(DB_PASSWORD);

void HeatExchangerData::acquireData(const char* json) {

	data.Parse(json);

	Value& v_status = data["status"];
	Value& v_supply_temp = data["supply_temp"];
	Value& v_returnMPC_temp = data["returnMPC_temp"];
	Value& v_timestamp = data["timestamp"];

	status.push_back(v_status.GetString());
	supply_temp.push_back(v_supply_temp.GetString());
	returnMPC_temp.push_back(v_returnMPC_temp.GetString());
	timestamp.push_back(v_timestamp.GetString());

}


std::string HeatExchangerData::getStatus() {

	return *(--status.end());

}

std::string HeatExchangerData::getSupplyTemp() {

	return *(--supply_temp.end());

}

std::string HeatExchangerData::getReturnMPCTemp() {

	return *(--returnMPC_temp.end());

}

std::string HeatExchangerData::getTimestamp() {

	return *(--timestamp.end());

}

void HeatExchangerData::insertLastIntoDB() {



	pqxx::connection C(params.c_str());
	pqxx::work W(C);

	const std::string	status = getStatus(),
						supply_temp = getSupplyTemp(),
						returnMPC_temp = getReturnMPCTemp(),
						timestamp = getTimestamp();

	W.exec0(
			"INSERT INTO exchanger (status, supply_temp, returnMPC_temp, timestamp) "
			"VALUES (" + /*W.quote(id) + ", " +*/
			W.quote(status) + ", " +
			W.quote(supply_temp) + ", " +
			W.quote(returnMPC_temp) +", " +
			W.quote(timestamp) + ")"
	);

	W.commit();

	purgeLastEntry();
}


void HeatExchangerData::purgeLastEntry() {
	status.pop_back();
	supply_temp.pop_back();
	returnMPC_temp.pop_back();
	timestamp.pop_back();
}

void HeatExchangerData::initDBTable() {
	pqxx::connection C(params.c_str());
	pqxx::work W(C);

	if(C.is_open())
			std::cout << "DB Succesfull" << std::endl;

	std::string cmd =
			"CREATE TABLE exchanger ("
			"id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY, "
			"status VARCHAR(20), "
			"supply_temp VARCHAR(20), "
			"returnMPC_temp VARCHAR(20), "
			"timestamp VARCHAR(40)"
			");";

	W.exec(cmd);

	W.commit();



}
