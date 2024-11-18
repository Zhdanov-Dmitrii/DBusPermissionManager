#include "PermissionsDatabase.hpp"

bool PermissionsDatabase::initialize(const std::string& dbPath) {
	sqlite3* db = nullptr;
	if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
		std::cerr << "Не удалось открыть базу данных." << std::endl;
		return false;
	}
	db_.reset(db);
	const char* createTableSQL = "CREATE TABLE IF NOT EXISTS Permissions ("
		"app_path TEXT, "
		"permission_code INTEGER)";
	if (sqlite3_exec(db_.get(), createTableSQL, nullptr, nullptr, nullptr) !=
		SQLITE_OK) {
		std::cerr << "Не удалось создать таблицу Permissions." << std::endl;
		return false;
	}
	return true;
}

bool PermissionsDatabase::isValid() const { return db_ != nullptr; }

bool PermissionsDatabase::addPermissionRequest(const std::string& appPath,
	int permissionCode) {
	if (!isValid()) {
		std::cerr << "База данных не инициализирована." << std::endl;
		return false;
	}

	const char* insertSQL =
		"INSERT INTO Permissions (app_path, permission_code) VALUES (?, ?)";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db_.get(), insertSQL, -1, &stmt, nullptr) !=
		SQLITE_OK) {
		std::cerr << "Ошибка подготовки SQL-запроса для добавления разрешения."
			<< std::endl;
		return false;
	}
	std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmtPtr(
		stmt, sqlite3_finalize);

	sqlite3_bind_text(stmt, 1, appPath.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, permissionCode);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::cerr << "Ошибка выполнения SQL-запроса для добавления разрешения."
			<< std::endl;
		return false;
	}
	return true;
}

bool PermissionsDatabase::hasPermission(const std::string& appPath,
	int permissionCode) {
	if (!isValid()) {
		std::cerr << "База данных не инициализирована." << std::endl;
		return false;
	}

	const char* querySQL = "SELECT COUNT(*) FROM Permissions WHERE app_path = ? "
		"AND permission_code = ?";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db_.get(), querySQL, -1, &stmt, nullptr) !=
		SQLITE_OK) {
		std::cerr << "Ошибка подготовки SQL-запроса для проверки разрешения."
			<< std::endl;
		return false;
	}
	std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmtPtr(
		stmt, sqlite3_finalize);

	sqlite3_bind_text(stmt, 1, appPath.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, permissionCode);

	bool hasPermission = false;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		hasPermission = (sqlite3_column_int(stmt, 0) > 0);
	}

	return hasPermission;
}
