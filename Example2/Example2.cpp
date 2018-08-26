#include <Listener.h>

using namespace std;

map<string, int> playerMoney = {
	{ "Panakotta00", 1237 },
	{ "Meep", 57234 }
};

int main() {
	Listener l("http://192.168.1.5:8080/api");
	l.path("/players/{playerID:str}").addRef("playerID", playerMoney).get([](auto req, auto vars) {
		req.reply(200, "{\"id\":" + to_string(any_cast<int>(vars[1])) + "}");
	});
	l.run();
}