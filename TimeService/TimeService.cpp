#include "TimeService.hpp"
#include <chrono>
#include <iostream>

TimeService::TimeService(sdbus::IConnection& connection)
	: object_{ sdbus::createObject(connection, sdbus::ObjectPath{"/"}) } {
	object_
		->addVTable(sdbus::MethodVTableItem{ sdbus::MethodName{"GetSystemTime"},
											{},
											{},
											sdbus::Signature{"i"},
											{"systemtime"},
											[this](sdbus::MethodCall call) {
											  return getSystemTime(
												  std::move(call));
											} })
		.forInterface("com.system.time");
}

std::string TimeService::getExecutablePath(const std::string& dbusName) {
	std::string cmd = "ps -p $(pgrep -f \"" + dbusName + "\") -o comm=";
	FILE* pipe = popen(cmd.c_str(), "r");
	if (!pipe) {
		return "";
	}

	char buffer[128];
	std::string result;
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		result += buffer;
	}
	pclose(pipe);

	result.erase(result.find_last_not_of(" \n\r\t") + 1);
	return result;
}

uint64_t TimeService::getSystemTimestamp() {
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

bool TimeService::checkPermission(const std::string& appPath,
	const std::string& permission) {
	try {
		auto proxy = sdbus::createProxy(
			sdbus::createSessionBusConnection(),
			sdbus::ServiceName{ "com.system.permissions" }, sdbus::ObjectPath{ "/" });

		bool hasPermission = false;
		proxy->callMethod("CheckApplicationHasPermission")
			.onInterface("com.system.permissions")
			.withArguments(appPath, 0)
			.storeResultsTo(hasPermission);

		return hasPermission;
	}
	catch (const sdbus::Error& e) {
		std::cerr << "Ошибка при проверке разрешения: " << e.what() << std::endl;
		return false;
	}
}

void TimeService::getSystemTime(sdbus::MethodCall call) {
	sdbus::MethodReply reply = call.createReply();

	auto dbusName = call.getSender();
	std::string appPath = getExecutablePath(dbusName);

	if (appPath.empty()) {
		auto error = call.createErrorReply(
			sdbus::Error{ sdbus::Error::Name{"com.system.time.Error"},
						 "Не удалось определить путь к исполняемому файлу." });
		error.send();
		return;
	}

	if (!checkPermission(appPath, "SystemTime")) {
		auto error = call.createErrorReply(
			sdbus::Error{ sdbus::Error::Name{"com.system.time.UnauthorizedAccess"},
						 "Отказано в доступе: нет разрешения SystemTime." });
		error.send();
		return;
	}

	uint64_t timestamp = getSystemTimestamp();
	reply << timestamp;
	reply.send();
}
