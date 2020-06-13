#include "eventListSystem.h"
#include <cassert> 
#include <cstring>
#include <sstream> 
#include <thread>
#include <algorithm>

static eventListSystem* eventList;

eventListSystem* eventListSystem::getInstance()
{
	if (!eventList) {
		eventList = new eventListSystem();
		assert(eventList);
		if (!eventList) {
			delete eventList;
			return nullptr;
		}
	}
	return eventList;
}

eventListSystem::eventListSystem()
{
	mapping["LEFT_PRESS"] = LEFT_PRESS;
	mapping["LEFT_PRESSED"] = LEFT_PRESSED;
	mapping["RIGHT_PRESS"] = RIGHT_PRESS;
	mapping["RIGHT_PRESSED"] = RIGHT_PRESSED;
	mapping["UP_PRESS"] = UP_PRESS;
	mapping["UP_PRESSED"] = UP_PRESSED;
	mapping["DOWN_PRESS"] = DOWN_PRESS;
	mapping["DOWN_PRESSED"] = DOWN_PRESSED;
	mapping["A_PRESSED"] = A_PRESSED;
	mapping["B_PRESSED"] = B_PRESSED;
	mapping["CONNECTED"] = CONNECTED;
}

eventListSystem::~eventListSystem()
{

}

std::vector<std::string> eventListSystem::split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

void eventListSystem::requestData(std::string data) {
	if (strchr(data.c_str(), ':')) {
		std::vector<std::string> data_split = split(data, ':');
		if (data_split.size() == 2) {
			for (auto callback : callbackEvents)
				callback.second(data_split.front(), (eEventList)mapping[data_split.back()]);
		}
	}
}

void eventListSystem::addEventCallback(std::string id, std::function<void(std::string, eEventList)> callback) {
	callbackEvents[id] = callback;
}

void eventListSystem::removeEventCallbackById(std::string id) {
	callbackEvents.erase(id);
}