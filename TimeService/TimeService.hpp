#pragma once

#include <sdbus-c++/sdbus-c++.h>
#include <string>

class TimeService {
public:
	TimeService(sdbus::IConnection& connection);

private:
	std::unique_ptr<sdbus::IObject> object_;

	std::string getExecutablePath(const std::string& dbusName);
	uint64_t getSystemTimestamp();
	void getSystemTime(sdbus::MethodCall call);
	bool checkPermission(const std::string& appPath,
		const std::string& permission);
};
