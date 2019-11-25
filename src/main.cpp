#include <iostream>
#include <pistache/endpoint.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <pqxx/pqxx>


#include "BuildingData.hpp"
#include "ProviderData.hpp"
#include "HeatExchangerData.hpp"
#include "ControllerData.hpp"

using namespace Pistache;
using namespace rapidjson;

BuildingData buildings;
ProviderData provider;
HeatExchangerData heatex;
ControllerData controller;


std::string processPOST(JSONData* instance, std::string body) {
	instance->acquireData(body.c_str());
	try {
		instance->insertLastIntoDB();
	} catch (const std::exception &) {
		instance->initDBTable();
		instance->insertLastIntoDB();
	}

	const std::string ret = "Data logged with timestamp: " + instance->getTimestamp();
	instance->purgeAll();
	return ret;

}

void processGETOne(StringBuffer* s, pqxx::result* row_data) {
	Writer<StringBuffer> writer(*s);

	std::vector<std::string> names, values;

	for(int i = 1; i<row_data->columns(); i++) {
		names.push_back(row_data->column_name(i));
		values.push_back(row_data->at(0).at(i).as<std::string>());
	}

	writer.StartObject();

	for(int i = 0;i<names.size();i++) {
		writer.String(names[i].c_str(),names[i].length());
		writer.String(values[i].c_str(),values[i].length());
	}

	writer.EndObject();

}

void processGETMany(StringBuffer* s, pqxx::result* res_data) {
	Writer<StringBuffer> writer(*s);

	std::vector<std::string> names, values;
	writer.StartArray();

	for(int i = res_data->size()-1; i>=0 ; i--) {
		for(int j = 1; j<res_data->columns(); j++) {
			names.push_back(res_data->column_name(j));
			values.push_back(res_data->at(i).at(j).as<std::string>());
		}

		writer.StartObject();

		for(int j = 0;j<names.size();j++) {
			writer.String(names[j].c_str(),names[j].length());
			writer.String(values[j].c_str(),values[j].length());
		}

		writer.EndObject();

		names.clear();
		values.clear();
	}
	writer.EndArray();
}

int getIntFromRequest(std::string request) {
	int length = 0;
	for(int i=request.length();request.c_str()[i]!='/';i--) {
		length++;
	}
	return std::stoi(request.substr(request.length()-length+1, length-1));
}

std::string getStrFromRequest(std::string request) {
	int length = 0;
	for(int i=request.length();request.c_str()[i]!='/';i--) {
		length++;
	}

	return request.substr(request.length()-length+1, length-1);
}

class TestHandler : public Http::Handler {
private:
	std::string body;
public:

    HTTP_PROTOTYPE(TestHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override {
    	if (request.method() == Http::Method::Post) {
    		if (request.resource() == "/exchanger/log") {

    			try {
    				body = request.body();
    				if (body == "")
    					throw "JSON error";
    			} catch (...){
    				auto res = response.send(Http::Code::Unprocessable_Entity,"Error while parsing JSON");
    				return;
    			}

    			const std::string s_response = processPOST(&heatex, body);

    			auto res = response.send(Http::Code::Ok,s_response.c_str());

    		} else if (request.resource() == "/controler/log") {

    			try {
					body = request.body();
					if (body == "")
						throw "JSON error";
				} catch (...){
					auto res = response.send(Http::Code::Unprocessable_Entity,"Error while parsing JSON");
					return;
				}
				const std::string s_response = processPOST(&controller, body);

				auto res = response.send(Http::Code::Ok,s_response.c_str());

    		} else if (request.resource() == "/building/log") {

    			try {
					body = request.body();
					if (body == "")
						throw "JSON error";
				} catch (...){
					auto res = response.send(Http::Code::Unprocessable_Entity,"Error while parsing JSON");
					return;
				}
				const std::string s_response = processPOST(&buildings, body);

				auto res = response.send(Http::Code::Ok,s_response.c_str());

    		} else if (request.resource() == "/provider/log") {

				try {
					body = request.body();
					if (body == "")
						throw "JSON error";
				} catch (...){
					auto res = response.send(Http::Code::Unprocessable_Entity,"Error while parsing JSON");
					return;
				}
				const std::string s_response = processPOST(&provider, body);

				auto res = response.send(Http::Code::Ok,s_response.c_str());

			} else {
    			response.send(Pistache::Http::Code::Not_Found,"Error 404: Called nonexistent resource");
    		}

    	} else if (request.method() == Http::Method::Get) {
    		if(request.resource().find("/exchanger") != std::string::npos ) {
    			if(request.resource().find("/id") != std::string::npos) {

    				int id = getIntFromRequest(request.resource());

    				pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result row_data = W.exec(
							"SELECT * FROM exchanger WHERE id = "
									+ W.quote(id));
					W.commit();

					StringBuffer s;

					processGETOne(&s, &row_data);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

    			} else if ((request.resource().find("/num") != std::string::npos)) {

					int num = getIntFromRequest(request.resource());

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result res = W.exec(
							"SELECT * FROM exchanger ORDER BY id DESC LIMIT "
									+ W.quote(num));
					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);



    			} else {		// GET FULL LOG

    				pqxx::connection C(params.c_str());
					pqxx::work W(C);

					pqxx::result res = W.exec(
							"SELECT * FROM exchanger ORDER BY id DESC");

					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

    			}
    		} else if(request.resource().find("/controler") != std::string::npos ) {
    			if(request.resource().find("/id") != std::string::npos) {
    				int id = getIntFromRequest(request.resource());

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result row_data = W.exec(
							"SELECT * FROM controler WHERE id = "
									+ W.quote(id));
					W.commit();

					StringBuffer s;

					processGETOne(&s, &row_data);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				} else if ((request.resource().find("/num") != std::string::npos)) {
					int num = getIntFromRequest(request.resource());

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result res = W.exec(
							"SELECT * FROM controler ORDER BY id DESC LIMIT "
									+ W.quote(num));
					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				} else {		// GET FULL LOG
    				pqxx::connection C(params.c_str());
					pqxx::work W(C);

					pqxx::result res = W.exec(
							"SELECT * FROM controler ORDER BY id DESC");

					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				}

    		} else if(request.resource().find("/building") != std::string::npos ) {
				if(request.resource().find("/id") != std::string::npos) {
					int id = getIntFromRequest(request.resource());

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result row_data = W.exec(
							"SELECT * FROM controler WHERE id = "
									+ W.quote(id));
					W.commit();

					StringBuffer s;

					processGETOne(&s, &row_data);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				} else if ((request.resource().find("/num") != std::string::npos)) {
					int num = getIntFromRequest(request.resource());

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result res = W.exec(
							"SELECT * FROM building  ORDER BY id DESC LIMIT "
									+ W.quote(num));
					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				} else if ((request.resource().find("/tag") != std::string::npos)) {
					std::string tag = getStrFromRequest(request.resource());

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result res = W.exec(
							"SELECT * FROM building "
							"WHERE tag_name = " + W.quote(tag) +
							" ORDER BY id DESC" );
					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				} else if ((request.resource().find("/taglast") != std::string::npos)) {
					int num = getIntFromRequest(request.resource());
					const std::string 	numS = std::to_string(num),
										req_mod = request.resource().substr(0,
									request.resource().length() - numS.length()-1);
					std::string tag = getStrFromRequest(req_mod);

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result res = W.exec(
							"SELECT * FROM building "
							"WHERE tag_name = " + W.quote(tag) +
							" ORDER BY id DESC "
							"LIMIT " + W.quote(num));
					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				} else {		// GET FULL LOG
					pqxx::connection C(params.c_str());
					pqxx::work W(C);

					pqxx::result res = W.exec(
							"SELECT * FROM building ORDER BY id DESC");

					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				}

    		} else if(request.resource().find("/provider") != std::string::npos ) {
				if(request.resource().find("/id") != std::string::npos) {
					int id = getIntFromRequest(request.resource());

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result row_data = W.exec(
							"SELECT * FROM provider WHERE id = "
									+ W.quote(id));
					W.commit();

					StringBuffer s;

					processGETOne(&s, &row_data);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				} else if ((request.resource().find("/num") != std::string::npos)) {
					int num = getIntFromRequest(request.resource());

					pqxx::connection C(params.c_str());
					pqxx::work W(C);
					pqxx::result res = W.exec(
							"SELECT * FROM provider ORDER BY id DESC LIMIT "
									+ W.quote(num));
					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				} else {		// GET FULL LOG
					pqxx::connection C(params.c_str());
					pqxx::work W(C);

					pqxx::result res = W.exec(
							"SELECT * FROM provider ORDER BY id DESC");

					W.commit();

					StringBuffer s;

					processGETMany(&s, &res);

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

				}

			} else {
    			response.send(Http::Code::Not_Found, "Error 404: Called nonexistent resource");
    		}
    	} else {
    		response.send(Http::Code::Method_Not_Allowed,"Access denied");
    	}

}

};


int main() {

	Address addr(IP(0,0,0,0),Port(8080));
	auto opts = Http::Endpoint::options()
								.threads(2)
								.flags(Tcp::Options::ReuseAddr);

	pqxx::connection c(params.c_str());

	if(c.is_open())
		std::cout << "DB Succesfull" << std::endl;


	Http::Endpoint server(addr);
	server.init(opts);

	std::cout << "SERVICE STARTED" << std::endl;



	server.setHandler(Http::make_handler<TestHandler>());
	server.serve();
}
