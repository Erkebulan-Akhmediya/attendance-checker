#include "db.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

Db* Db::m_instance;

Db& Db::get_instance()
{
    if (!m_instance)
        m_instance = new Db();
    return *m_instance;
}

Db::Db()
    : m_db(nullptr, &sqlite3_close)
{
    sqlite3* db = nullptr;
    const int res = sqlite3_open(std::getenv("DB_FILE"), &db);
    if (res != SQLITE_OK)
        throw std::runtime_error("failed to open database connection");
    std::cout << "successfully connected to database (" << std::getenv("DB_FILE") << ")\n";
    m_db.reset(db);
}

std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> Db::make_stmt(const std::string& query)
{
    sqlite3_stmt* raw_stmt = nullptr;
    sqlite3_prepare_v2(m_db.get(), query.c_str(), -1, &raw_stmt, nullptr);
    return { raw_stmt, &sqlite3_finalize };
}

int Db::rows_changed()
{
    return sqlite3_changes(m_db.get());
}

int write_row(void* rows, int cols, char** col, char**)
{
    if (!col)
        return 0;
    auto rows_vector = reinterpret_cast<std::vector<std::vector<std::string>>*>(rows);
    std::vector<std::string> row;
    for (int i = 0; i < cols; ++i)
        row.push_back(col[i]);
    rows_vector->push_back(row);
    return 0;
}

std::vector<std::vector<std::string>> Db::exec(const std::string&& query)
{
    std::vector<std::vector<std::string>> rows;
    std::unique_ptr<char, decltype(&sqlite3_free)> err_msg(nullptr, &sqlite3_free);
    char* err_msg_data = err_msg.get();
    const int res = sqlite3_exec(m_db.get(), query.data(), &write_row, &rows, &err_msg_data);
    if (res != SQLITE_OK) {
        std::cout << err_msg.get() << '\n';
        throw std::runtime_error("failed to exec");
    }
    return rows;
}
