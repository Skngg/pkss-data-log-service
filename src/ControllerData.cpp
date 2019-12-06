#include "ControllerData.hpp"

using namespace rapidjson;

void ControllerData::insertLastIntoDB() {



	pqxx::connection C(params.c_str());
	pqxx::work W(C);


//	std::string cmd =	"INSERT INTO controler"
//						"(status, incoming_water_temp_Tzco, set_temp_Tzcoref, valve, timestamp) "
//						"VALUES (";

	std::string cmd =	"INSERT INTO controler (";
	for(auto it : parsed_data) {
		cmd += it.first;
		if(it.first != (--parsed_data.end())->first) {
			cmd += ", ";
		} else {
			cmd+=") ";
		}
	}

	cmd+="VALUES (";

	for (auto it : parsed_data) {
		cmd += W.quote(it.second) +", ";
	}
	cmd.erase(cmd.end()-2,cmd.end()-1);
	cmd+= ")";

	W.exec(cmd);

	W.commit();
}

void ControllerData::initDBTable() {
	pqxx::connection C(params.c_str());
	pqxx::work W(C);

	std::string cmd =
			"CREATE TABLE controler ("
			"id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY, "
			"status VARCHAR(20), "
			"incoming_water_temp_Tzco VARCHAR(20), "
			"set_temp_Tzcoref VARCHAR(20), "
			"valve VARCHAR(20), "
			"timestamp VARCHAR(40)"
			");";

	W.exec(cmd);

	W.commit();

}
