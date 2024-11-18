#include <iostream>
#include <sdbus-c++/sdbus-c++.h>

bool requestPermission() {
	try {
		auto proxy = sdbus::createProxy(
			sdbus::createSessionBusConnection(),
			sdbus::ServiceName{ "com.system.permissions" }, sdbus::ObjectPath{ "/" }
		);

		// Пытаемся запросить разрешение SystemTime у com.system.permissions
		proxy->callMethod("RequestPermission")
			.onInterface("com.system.permissions")
			.withArguments(static_cast<int>(0)); // SystemTime = 0
		std::cout << "Запрос разрешения SystemTime выполнен успешно." << std::endl;
		return true;
	}
	catch (const sdbus::Error& e) {
		std::cerr << "Ошибка при запросе разрешения: " << e.what() << std::endl;
		return false;
	}
}

bool getTime() {
	try {
		auto proxy = sdbus::createProxy(sdbus::createSessionBusConnection(),
			sdbus::ServiceName{ "com.system.time" },
			sdbus::ObjectPath{ "/" });

		uint64_t timestamp = 0;
		proxy->callMethod("GetSystemTime")
			.onInterface("com.system.time")
			.storeResultsTo(timestamp);

		std::cout << "Текущее время (timestamp): " << timestamp << std::endl;
		return true;
	}
	catch (const sdbus::Error& e) {
		if (e.getName() == "com.system.time.UnauthorizedAccess") {
			std::cerr << "Ошибка: недостаточно прав для получения времени "
				"(UnauthorizedAccess)."
				<< std::endl;
			return false;
		}
		else {
			std::cerr << "Произошла ошибка: " << e.what() << std::endl;
			return true;
		}
	}
}

int main() {
	std::cout << "Запрос текущего системного времени..." << std::endl;

	if (!getTime()) {
		std::cout << "Попытка запроса разрешения..." << std::endl;

		if (requestPermission()) {
			std::cout << "Повторный запрос текущего системного времени..."
				<< std::endl;
			getTime();
		}
		else {
			std::cerr << "Не удалось запросить разрешение." << std::endl;
		}
	}

	return 0;
}
