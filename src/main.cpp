#include <iostream>
#include <pistache/endpoint.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <pqxx/pqxx>


#include "HeatExchangerData.hpp"
#include "JSONData.hpp"


using namespace Pistache;
using namespace rapidjson;

HeatExchangerData heatex1;

class TestHandler : public Http::Handler {
private:
	std::string body;
public:

    HTTP_PROTOTYPE(TestHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override {
    	if (request.method() == Http::Method::Post) {
    		if (request.resource() == "/heatex/log") {

    			body = request.body();
    			heatex1.acquireData(body.c_str());

    			heatex1.insertLastIntoDB();

    			auto res = response.send(Http::Code::Ok,heatex1.getTempString().c_str());

    		}

    		else if (request.resource() == "/heatprov") {

    		} else {
    			response.send(Pistache::Http::Code::Not_Found,"Called nonexistent resource\n");
    		}

    	} else if (request.method() == Http::Method::Get) {
    		if(request.resource() == "/heatex") {

    			std::map<std::string,float> logs = heatex1.getFullLog();

    			StringBuffer s;
				Writer<StringBuffer> writer(s);

				writer.StartArray();

				const std::string time("timestamp");
				const std::string val("COTemp");

				for(std::map<std::string,float>::iterator it = logs.begin();it!=logs.end();it++) {
					writer.StartObject();
					const std::string tmp = it->first;
					const float tmp_val = it->second;
					writer.String(time.c_str(), static_cast<SizeType>(time.length()));
					writer.String(tmp.c_str(),static_cast<SizeType>(tmp.length()));

					writer.String(val.c_str(),static_cast<SizeType>(val.length()));
					writer.Double(tmp_val);
					writer.EndObject();
				}

				writer.EndArray();

    			std::string out = s.GetString();


				auto mimeType = MIME(Application, Json);


    			response.send(Http::Code::Ok, out, mimeType);

    		}
    	} else {
    		response.send(Http::Code::Method_Not_Allowed,"Access denied");
    	}

}

};


int main() {

	Address addr(Ipv4::any(),Port(8080));
	auto opts = Http::Endpoint::options().threads(1);

	pqxx::connection c("host = localhost dbname = pkssdatalog user = pkssservice password = kolek");

	if(c.is_open())
		std::cout << "DB Succesfull" << std::endl;


	Http::Endpoint server(addr);
	server.init(opts);

	std::cout << "SERVICE STARTED" << std::endl;



	server.setHandler(Http::make_handler<TestHandler>());
	server.serve();
}
