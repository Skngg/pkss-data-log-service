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

static const std::string params =	"host = " + std::string(DB_HOST) +
									" dbname = " + std::string(DB_NAME) +
									" user = " + std::string(DB_USER) +
									" password = " + std::string(DB_PASSWORD);

HeatExchangerData heatex;

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
    			heatex.acquireData(body.c_str());

    			const std::string s_response = "Data logged with timestamp: " + heatex.getTimestamp();

    			try {
    				heatex.insertLastIntoDB();
    			} catch (const std::exception &) {
					heatex.initDBTable();
					heatex.insertLastIntoDB();
				}

    			auto res = response.send(Http::Code::Ok,s_response.c_str());

    		} else if (request.resource() == "/provider/log") {

    			body = request.body();


    		} else {
    			response.send(Pistache::Http::Code::Not_Found,"Called nonexistent resource\n");
    		}

    	} else if (request.method() == Http::Method::Get) {
    		if(request.resource().find("/exchanger") != std::string::npos ) {
    			if(request.resource().find("/id") != std::string::npos) {
    				const std::string req = request.resource();
    				int length = 0;
    				for(int i=req.length();req.c_str()[i]!='/';i--) {
    					length++;
    				}
    				int id = std::stoi(req.substr(req.length()-length+1, length-1));

    				pqxx::connection C(params.c_str());
					pqxx::work W(C);

					pqxx::row row_data = W.exec1(
							"SELECT * FROM exchanger WHERE id = "
									+ W.quote(id));

					W.commit();

					StringBuffer s;
					Writer<StringBuffer> writer(s);

					const std::string	s_status("status"),
										s_supply_temp("supply_temp"),
										s_returnMPC_temp("returnMPC_temp"),
										s_timestamp("timestamp");

					const std::string	v_status = row_data[1].as<std::string>(),
										v_supply_temp = row_data[2].as<std::string>(),
										v_returnMPC_temp = row_data[3].as<std::string>(),
										v_timestamp = row_data[4].as<std::string>();

					writer.StartObject();

					writer.String(s_status.c_str(),s_status.length());
					writer.String(v_status.c_str(),v_status.length());

					writer.String(s_supply_temp.c_str(),s_supply_temp.length());
					writer.String(v_supply_temp.c_str(),v_supply_temp.length());

					writer.String(s_returnMPC_temp.c_str(),s_returnMPC_temp.length());
					writer.String(v_returnMPC_temp.c_str(),v_returnMPC_temp.length());

					writer.String(s_timestamp.c_str(),s_timestamp.length());
					writer.String(v_timestamp.c_str(),v_timestamp.length());

					writer.EndObject();

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

    			} else if ((request.resource().find("/num") != std::string::npos)) {
    				const std::string req = request.resource();
					int length = 0;
					for(int i=req.length();req.c_str()[i]!='/';i--) {
						length++;
					}
					int num = std::stoi(req.substr(req.length()-length+1, length-1));

					pqxx::connection C(params.c_str());
					pqxx::work W(C);

					pqxx::result res = W.exec(
							"SELECT * FROM exchanger ORDER BY id DESC LIMIT "
									+ W.quote(num));

					W.commit();

					StringBuffer s;
					Writer<StringBuffer> writer(s);

					const std::string	s_status("status"),
										s_supply_temp("supply_temp"),
										s_returnMPC_temp("returnMPC_temp"),
										s_timestamp("timestamp");

					std::string	v_status,
								v_supply_temp,
								v_returnMPC_temp,
								v_timestamp;

					writer.StartArray();

					for (auto row_data : res) {
						writer.StartObject();
						v_status = row_data[1].as<std::string>();
						v_supply_temp = row_data[2].as<std::string>();
						v_returnMPC_temp = row_data[3].as<std::string>();
						v_timestamp = row_data[4].as<std::string>();

						writer.String(s_status.c_str(),s_status.length());
						writer.String(v_status.c_str(),v_status.length());

						writer.String(s_supply_temp.c_str(),s_supply_temp.length());
						writer.String(v_supply_temp.c_str(),v_supply_temp.length());

						writer.String(s_returnMPC_temp.c_str(),s_returnMPC_temp.length());
						writer.String(v_returnMPC_temp.c_str(),v_returnMPC_temp.length());

						writer.String(s_timestamp.c_str(),s_timestamp.length());
						writer.String(v_timestamp.c_str(),v_timestamp.length());

						writer.EndObject();
					}

					writer.EndArray();

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);



    			} else {		// GET FULL LOG

    				pqxx::connection C(params.c_str());
					pqxx::work W(C);

					pqxx::result res = W.exec(
							"SELECT * FROM exchanger ORDER BY id DESC");

					W.commit();

					StringBuffer s;
					Writer<StringBuffer> writer(s);

					const std::string	s_status("status"),
										s_supply_temp("supply_temp"),
										s_returnMPC_temp("returnMPC_temp"),
										s_timestamp("timestamp");

					std::string	v_status,
								v_supply_temp,
								v_returnMPC_temp,
								v_timestamp;

					writer.StartArray();

					for (auto row_data : res) {
						writer.StartObject();
						v_status = row_data[1].as<std::string>();
						v_supply_temp = row_data[2].as<std::string>();
						v_returnMPC_temp = row_data[3].as<std::string>();
						v_timestamp = row_data[4].as<std::string>();

						writer.String(s_status.c_str(),s_status.length());
						writer.String(v_status.c_str(),v_status.length());

						writer.String(s_supply_temp.c_str(),s_supply_temp.length());
						writer.String(v_supply_temp.c_str(),v_supply_temp.length());

						writer.String(s_returnMPC_temp.c_str(),s_returnMPC_temp.length());
						writer.String(v_returnMPC_temp.c_str(),v_returnMPC_temp.length());

						writer.String(s_timestamp.c_str(),s_timestamp.length());
						writer.String(v_timestamp.c_str(),v_timestamp.length());

						writer.EndObject();
					}

					writer.EndArray();

					auto mimeType = MIME(Application, Json);
					response.send(Http::Code::Ok, s.GetString(), mimeType);

    			}
    		} else {
    			response.send(Http::Code::Not_Found, "Called nonexistent resource\n");
    		}
    	} else {
    		response.send(Http::Code::Method_Not_Allowed,"Access denied");
    	}

}

};


int main() {

	Address addr(IP(0,0,0,0),Port(8080));
	auto opts = Http::Endpoint::options()
								.threads(1)
								.flags(Tcp::Options::ReuseAddr);

	std::cout << addr.host() << std::endl;

//	pqxx::connection c("host = logs.cegwdkw512mn.us-east-2.rds.amazonaws.com dbname = logs_mk user = pkssAdmin password = pkssAdmin1");

	pqxx::connection c(params.c_str());

	if(c.is_open())
		std::cout << "DB Succesfull" << std::endl;


	Http::Endpoint server(addr);
	server.init(opts);

	std::cout << "SERVICE STARTED" << std::endl;



	server.setHandler(Http::make_handler<TestHandler>());
	server.serve();
}
