#ifndef _EVENT_LIST_SYSTEM_
#define _EVENT_LIST_SYSTEM_

#include <string>
#include <vector>
#include <map>
#include <functional>

class eventListSystem {
	public:
		enum eEventList {
			LEFT_PRESS = 0,
			LEFT_PRESSED,
			RIGHT_PRESS,
			RIGHT_PRESSED,
			UP_PRESS,
			UP_PRESSED,
			DOWN_PRESS,
			DOWN_PRESSED,
			A_PRESSED,
			B_PRESSED,
			CONNECTED
		};

		std::map<std::string, int> mapping;

		static eventListSystem* getInstance();
		eventListSystem();
		~eventListSystem();

		void requestData(std::string data);
		void addEventCallback(std::string id, std::function<void(std::string, eEventList)> callback);
		void removeEventCallbackById(std::string id);
	private:
		std::vector<std::string> split(const std::string& s, char delimiter);

		std::map<std::string, std::function<void(std::string playerId, eEventList event)>> callbackEvents;
};

#endif