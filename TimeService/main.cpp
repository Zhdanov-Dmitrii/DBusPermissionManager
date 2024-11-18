#include "TimeService.hpp"
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <thread>

int main() {
	try {
		auto connection = sdbus::createSessionBusConnection();
		connection->requestName(sdbus::ServiceName{ "com.system.time" });

		TimeService service(*connection);

		std::cout << "DBus сервис com.system.time запущен..." << std::endl;

		while (true) {
			connection->processPendingEvent();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	catch (const std::exception& ex) {
		std::cerr << "Ошибка: " << ex.what() << std::endl;
		return 1;
	}
}
