#include <iostream>
#include <webcpp/webcpp.hpp>
#include <sqlite3.h>

struct User {
    int id;
    std::string name;
    int age;
};

int main() {
    webcpp::Application app;

    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Не удалось открыть базу данных" << std::endl;
        return 1;
    }

    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS User (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Не удалось создать таблицу User" << std::endl;
        sqlite3_close(db);
        return 1;
    }

    app.get("/users", [db](webcpp::Request& req, webcpp::Response& res) {
        std::vector<User> users;
        sqlite3_stmt* stmt;
        const char* selectSQL = "SELECT id, name, age FROM User;";
        rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                User user;
                user.id = sqlite3_column_int(stmt, 0);
                user.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                user.age = sqlite3_column_int(stmt, 2);
                users.push_back(user);
            }
            sqlite3_finalize(stmt);
        }
        res.json(users);
    });

    app.get("/users/:id", [db](webcpp::Request& req, webcpp::Response& res) {
        int id = std::stoi(req.params["id"]);
        User user;
        sqlite3_stmt* stmt;
        const char* selectSQL = "SELECT id, name, age FROM User WHERE id = ?;";
        rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                user.id = sqlite3_column_int(stmt, 0);
                user.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                user.age = sqlite3_column_int(stmt, 2);
                res.json(user);
            } else {
                res.status(404).json("Пользователь не найден");
            }
            sqlite3_finalize(stmt);
        } else {
            res.status(500).json("Ошибка при запросе к базе данных");
        }
    });

    app.post("/users", [db](webcpp::Request& req, webcpp::Response& res) {
        User user;
        if (req.json().is_object()) {
            user.name = req.json()["name"].as_string();
            user.age = req.json()["age"].as_int();
        } else {
            res.status(400).json("Неверный формат JSON");
            return;
        }

        sqlite3_stmt* stmt;
        const char* insertSQL = "INSERT INTO User (name, age) VALUES (?, ?);";
        rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, user.name.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 2, user.age);
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE) {
                user.id = sqlite3_last_insert_rowid(db);
                res.status(201).json(user);
            } else {
                res.status(500).json("Ошибка при вставке данных в базу данных");
            }
            sqlite3_finalize(stmt);
        } else {
            res.status(500).json("Ошибка при запросе к базе данных");
        }
    });

    app.listen(8080);
    sqlite3_close(db);

    return 0;
}
