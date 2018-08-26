#include "Listener.h"

using namespace std;
using namespace std::experimental;

int main() {
	Listener listener("http://192.168.1.5:8080/api");
	listener.path("/").get([=](auto req, auto vars) {
		req.reply(web::http::status_codes::OK, "/");
	});
	listener.path("/tasks").get([=](auto req, auto vars) {
		req.reply(web::http::status_codes::OK, "/tasks");
	});
	listener.path("/{meep:str}").get([=](auto req, auto vars) {
		cout << any_cast<string>(vars.at(0)) << endl;
		req.reply(web::http::status_codes::OK, "/{meep}");
	});
	listener.path("/tasks/{taskID:int}").get([=](auto req, auto vars) {
		cout << any_cast<int>(vars.at(0)) << endl;
		req.reply(web::http::status_codes::OK, "/tasks/{id}");
	}).get([=](web::http::http_request req, auto vars) {
		cout << any_cast<int>(vars.at(0)) << endl;
	});
	listener.path("/tasks/{taskID:int}/meep/{meepID:int}").get([=](auto req, auto vars) {
		cout << any_cast<int>(vars.at(0)) << " - " << any_cast<int>(vars.at(1)) << endl;
		req.reply(web::http::status_codes::OK, "/tasks/{id}/meep/{meepID:int}");
	});
	listener.path("/msgs/{msg:str}").get([=](auto req, auto vars) {
		cout << any_cast<string>(vars.at(0)) << endl;
		req.reply(web::http::status_codes::OK, "/msgs/{msg}");
	});
	listener.path("/users/{userID:int}").addRef<string>("userID", (std::vector<string>){ "string1", "string2" }).get([=](auto req, auto vars) {
		cout << "UserID : " << any_cast<int>(vars[0]) << endl << "User   : " << any_cast<string>(vars[1]) << endl;
	});
	listener.path("/accounts/{accID:int}")
		.addRef("accID", (RefFunc)[](http_request req, vector<any>& args, any id){
		args.push_back(to_string(any_cast<int>(id)));
		return 0;
	}).get([](http_request request, vector<any> vars) {
		cout << "AccIDs : " << any_cast<string>(vars[0]) << endl;
	});
	listener.path("/meeps/{meepUUID:str}").addRef<int>("meepUUID", { {"1a", 30}, {"2b", 5} }).get([](http_request request, vector<any> vars) {
		cout << "MeepUUID : " << any_cast<string>(vars[0]) << endl
			 << "MeepINT  : " << any_cast<int>(vars[1]) << endl;
	});
	listener.run();

	char e;
	cin >> e;
}