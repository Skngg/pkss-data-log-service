#ifndef JSONDATA_HPP_
#define JSONDATA_HPP_

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class JSONData {
protected:
	rapidjson::Document data;
public:

	void acquireData(const char* json) {};
};

#endif /* JSONDATA_HPP_ */
