#ifndef JSONDATA_HPP_
#define JSONDATA_HPP_

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <pqxx/pqxx>
#include <vector>

#define DB_HOST		"logs.cegwdkw512mn.us-east-2.rds.amazonaws.com"
#define DB_NAME		"logs_mk"
#define DB_USER		"pkssAdmin"
#define DB_PASSWORD	"pkssAdmin1"

class JSONData {
protected:
	rapidjson::Document data;
	std::vector<std::pair<std::string, std::string>> parsed_data;
public:
	void acquireData(const char* json);
	std::string getValue(int n);
	std::string getTimestamp();
	virtual void insertLastIntoDB() {};
	virtual void initDBTable() {};
	void purgeAll();
};

const std::string params =	"host = " + std::string(DB_HOST) +
							" dbname = " + std::string(DB_NAME) +
							" user = " + std::string(DB_USER) +
							" password = " + std::string(DB_PASSWORD);

#endif /* JSONDATA_HPP_ */
