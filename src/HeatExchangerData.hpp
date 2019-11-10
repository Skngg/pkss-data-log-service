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
	std::map<std::string,float> log;
	static unsigned int dbID;
public:
	void acquireData(const char* json);
	float getTemp();
	std::string getTempString();
	std::string getTimestamp();
	std::map<std::string,float> getFullLog();
	void insertLastIntoDB();
};

#endif /* HEATEXCHANGERDATA_HPP_ */
