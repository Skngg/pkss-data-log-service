#include "BuildingData.hpp"

using namespace rapidjson;

void BuildingData::insertLastIntoDB() {



	pqxx::connection C(params.c_str());
	pqxx::work W(C);


	std::string cmd =	"INSERT INTO building"
						"(status, tag_name, water_intake_Fcob, "
						"return_water_temp_Tpcob, radiator_temp_Th, "
						"room_temp_Tr, timestamp) "
						"VALUES (";

	for (auto it : parsed_data) {
		cmd += W.quote(it.second) +", ";
	}
	cmd.erase(cmd.end()-2,cmd.end()-1);
	cmd+= ")";

	W.exec(cmd);

	W.commit();
}

void BuildingData::initDBTable() {
	pqxx::connection C(params.c_str());
	pqxx::work W(C);

	std::string cmd =
			"CREATE TABLE building ("
			"id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY, "
			"status VARCHAR(20), "
			"tag_name VARCHAR(40), "
			"water_intake_Fcob VARCHAR(20), "
			"return_water_temp_Tpcob VARCHAR(20), "
			"radiator_temp_Th VARCHAR(20), "
			"room_temp_Tr VARCHAR(20), "
			"timestamp VARCHAR(40)"
			");";

	W.exec(cmd);

	W.commit();

}
