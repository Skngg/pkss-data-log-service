#include "ProviderData.hpp"

void ProviderData::insertLastIntoDB() {



	pqxx::connection C(params.c_str());
	pqxx::work W(C);


	std::string cmd =	"INSERT INTO provider"
						"(status, warm_water_stream_Fzm, incoming_water_temp_Tzm, outside_temp_To, failure, timestamp) "
						"VALUES (";

	for (auto it : parsed_data) {
		cmd += W.quote(it.second) +", ";
	}
	cmd.erase(cmd.end()-2,cmd.end()-1);
	cmd+= ")";

	W.exec(cmd);

	W.commit();
}

void ProviderData::initDBTable() {
	pqxx::connection C(params.c_str());
	pqxx::work W(C);

	std::string cmd =
			"CREATE TABLE provider ("
			"id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY, "
			"status VARCHAR(20), "
			"warm_water_stream_Fzm VARCHAR(20), "
			"incoming_water_temp_Tzm VARCHAR(20), "
			"outside_temp_To VARCHAR(20), "
			"failure VARCHAR(20), "
			"timestamp VARCHAR(40)"
			");";

	W.exec(cmd);

	W.commit();

}
