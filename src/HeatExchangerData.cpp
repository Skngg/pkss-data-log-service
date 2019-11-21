#include "HeatExchangerData.hpp"
#include <iostream>

using namespace rapidjson;

void HeatExchangerData::insertLastIntoDB() {



	pqxx::connection C(params.c_str());
	pqxx::work W(C);

	std::string cmd =	"INSERT INTO exchanger (status, supply_temp, returnMPC_temp, timestamp) "
						"VALUES (";

	for (auto it : parsed_data) {
		cmd += W.quote(it.second) +", ";
	}
	cmd.erase(cmd.end()-2,cmd.end()-1);
	cmd+= ")";

	W.exec0(cmd);

	W.commit();
}

void HeatExchangerData::initDBTable() {
	pqxx::connection C(params.c_str());
	pqxx::work W(C);

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
