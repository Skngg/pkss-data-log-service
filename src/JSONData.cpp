#include "JSONData.hpp"

void JSONData::acquireData(const char* json) {
	try {
		data.Parse(json);
		if (data.HasParseError())
			throw "JSON Error";
	} catch (...) {
		throw "JSON Error";
	}

	for( auto it = data.MemberBegin(); it != data.MemberEnd();it++) {
		parsed_data.push_back(std::pair<std::string,std::string>(it->name.GetString(),it->value.GetString()));
	}
}

std::string JSONData::getValue(int n) {
	return parsed_data.at(n).second;
}

std::string JSONData::getTimestamp() {
	return (parsed_data.end()-1)->second;
}

void JSONData::purgeAll() {
	parsed_data.clear();
}

