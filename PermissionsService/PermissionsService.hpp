#pragma once

#include "PermissionsDatabase.hpp"
#include <memory>
#include <sdbus-c++/sdbus-c++.h>
#include <string>

enum Permissions { SystemTime = 0 };

class PermissionsService {
public:
	PermissionsService(sdbus::IConnection& connection);
	bool
		initializeDatabase(const std::string& dbPath); // Инициализация базы данных

private:
	std::unique_ptr<sdbus::IObject> object_;
	PermissionsDatabase db_;

	std::string getExecutablePath(const std::string& dbusName);
	void requestPermission(sdbus::MethodCall call);
	bool checkApplicationHasPermission(sdbus::MethodCall call);
};
