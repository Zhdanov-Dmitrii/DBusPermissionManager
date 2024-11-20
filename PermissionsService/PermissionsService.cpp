#include "PermissionsService.hpp"
#include <iostream>
#include <sdbus-c++/Error.h>

PermissionsService::PermissionsService(sdbus::IConnection& connection)
	: object_{ sdbus::createObject(connection, sdbus::ObjectPath{"/"}) } {
	object_
		->addVTable(
			sdbus::MethodVTableItem{ sdbus::MethodName{"RequestPermission"},
									sdbus::Signature{"i"},
									{"permissionEnumCode"},
									{},
									{},
									[this](sdbus::MethodCall call) {
									  requestPermission(std::move(call));
									} },

			sdbus::MethodVTableItem{
				sdbus::MethodName{"CheckApplicationHasPermission"},
				sdbus::Signature{"si"},
				{"applicationExecPath", "permissionEnumCode"},
				sdbus::Signature{"b"},
				{"checkApplicationHasPermission"},
				[this](sdbus::MethodCall call) {
				  return checkApplicationHasPermission(std::move(call));
				} })
		.forInterface("com.system.permissions");
}

bool PermissionsService::initializeDatabase(const std::string& dbPath) {
	return db_.initialize(dbPath);
}

std::string PermissionsService::getExecutablePath(const std::string& dbusName) {
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

void PermissionsService::requestPermission(sdbus::MethodCall call) {
	if (!db_.isValid()) {
		auto error = call.createErrorReply(
			sdbus::Error{ sdbus::Error::Name{"com.system.permissions.Error"},
						 "База данных не инициализирована." });
		error.send();
		return;
	}

	int permissionEnumCode;
	call >> permissionEnumCode;

	auto dbusName = call.getSender();
	std::string appPath = getExecutablePath(dbusName);
	if (appPath.empty()) {
		auto error = call.createErrorReply(
			sdbus::Error{ sdbus::Error::Name{"com.system.permissions.Error"},
						 "Не удалось получить путь к исполняемому файлу." });
		error.send();
		return;
	}

	if (!db_.addPermissionRequest(appPath, permissionEnumCode)) {
		auto error = call.createErrorReply(sdbus::Error{
			sdbus::Error::Name{"com.system.permissions.Error"},
			"Не удалось записать запрос на разрешение в базу данных." });
		error.send();
		return;
	}
	call.createReply().send();
}

bool PermissionsService::checkApplicationHasPermission(sdbus::MethodCall call) {
	if (!db_.isValid()) {
		auto error = call.createErrorReply(
			sdbus::Error{ sdbus::Error::Name{"com.system.permissions.Error"},
						 "База данных не инициализирована." });
		error.send();
		return false;
	}

	std::string applicationExecPath;
	int permissionEnumCode;
	call >> applicationExecPath >> permissionEnumCode;

	bool hasPermission =
		db_.hasPermission(applicationExecPath, permissionEnumCode);
	auto reply = call.createReply();
	reply << hasPermission;
	reply.send();
	return hasPermission;
}
