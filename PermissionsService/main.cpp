#include "PermissionsService.hpp"
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <thread>

int main() {
	try {
		auto connection = sdbus::createSessionBusConnection();
		connection->requestName(sdbus::ServiceName{ "com.system.permissions" });

		PermissionsService service(*connection);

		if (!service.initializeDatabase("permissions.db")) {
			std::cerr << "Не удалось инициализировать базу данных." << std::endl;
			return 1;
		}

		std::cout << "DBus сервис запущен..." << std::endl;

		// Бесконечный цикл для обработки запросов
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
