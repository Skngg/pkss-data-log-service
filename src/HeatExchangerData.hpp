#ifndef HEATEXCHANGERDATA_HPP_
#define HEATEXCHANGERDATA_HPP_

#include <map>
#include <pqxx/pqxx>
#include "JSONData.hpp"

#define DB_HOST		"logs.cegwdkw512mn.us-east-2.rds.amazonaws.com"
#define DB_NAME		"logs_mk"
#define DB_USER		"pkssAdmin"
#define DB_PASSWORD	"pkssAdmin1"

class HeatExchangerData: public JSONData {
private:
	std::vector<std::string> status;
	std::vector<std::string> supply_temp;
	std::vector<std::string> returnMPC_temp;
	std::vector<std::string> timestamp;
//	static unsigned int dbID;
public:
	void acquireData(const char* json);
	std::string getStatus();
	std::string getSupplyTemp();
	std::string getReturnMPCTemp();
	std::string getTimestamp();
	void insertLastIntoDB();
	void purgeLastEntry();
	void initDBTable();
};

#endif /* HEATEXCHANGERDATA_HPP_ */
