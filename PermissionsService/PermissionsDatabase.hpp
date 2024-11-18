#pragma once

#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <string>

class PermissionsDatabase {
public:
	PermissionsDatabase();
	bool initialize(const std::string& dbPath);
	bool isValid() const;

	bool addPermissionRequest(const std::string& addPath, int permissionCode);
	bool hasPermission(const std::string& appPath, int permissionCode);

private:
	std::unique_ptr<sqlite3, decltype(&sqlite3_close)> db_{ nullptr,
														   sqlite3_close };
};
