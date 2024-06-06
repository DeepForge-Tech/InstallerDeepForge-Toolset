#ifndef DATABASEAPI_HPP_
#define DATABASEAPI_HPP_
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sqlite3/sqlite3.h>
#include <unordered_map>
#include <filesystem>
#include <typeinfo>
#include <map>

namespace DB
{
    typedef std::unordered_map<std::string, std::string> DatabaseValues;

    class Database
    {
    public:
        
        int ArraySize;
        sqlite3 *db;
        sqlite3_stmt *statement;
        bool isOpen = false;
        // std::string AnswerDB;
        std::string DefaultDatabesePath = std::filesystem::current_path().generic_string() + "/DB/AppInstaller.db";
        void open(std::string *DB_Path = nullptr)
        {
            int RESULT_SQL = sqlite3_open(DB_Path != nullptr ? DB_Path->c_str() : DefaultDatabesePath.c_str(), &db);

            // if result of open database != SQLITE_OK, that send error
            if (RESULT_SQL != SQLITE_OK)
            {
                throw std::runtime_error("Failed to connect to database");
            }
            isOpen = true;
        }
        // Database();
        ~Database()
        {
            close();
        }
        void close()
        {
            if (isOpen) sqlite3_close(db);
        }
        int CreateTable(const std::string &NameTable, DatabaseValues Columns);

        int InsertRowToTable(const std::string &NameTable, DatabaseValues  Fields);

        bool ExistRowInTable(const std::string &NameTable,const std::string &NameColumn,const std::string &Value);

        std::string GetValueFromRow(const std::string &NameTable, const std::string &NameColumn, std::unordered_map<std::string,std::string> Parameters);

        DatabaseValues GetRowByID(const std::string &NameTable,const int &id);

        std::unordered_map<int, DatabaseValues> GetRowFromTable(const std::string &NameTable, DatabaseValues  Parameters);

        std::unordered_map<int, DatabaseValues> GetAllRowsFromTable(const std::string &NameTable);

        std::unordered_map<std::string,std::string> GetMaxRowFromTable(const std::string &NameTable,const std::string &NameColumn, std::unordered_map<std::string,std::string> Parameters);

        std::string GetMaxValueFromTable(const std::string &NameTable, const std::string &NameColumn, std::unordered_map<std::string,std::string> Parameters);

        int RemoveRowFromTable(const std::string &NameTable,std::unordered_map<std::string,std::string> Parameters);

        int AddApplications(const std::string Tables[]);

        int RemoveApplications(const std::string Tables[]);
        
        int DeleteAllRows(const std::string  &NameTable);

        int RunQuery(const std::string  &SQL_QUERY);

        std::unordered_map<int, DatabaseValues> ExecuteQuery(const std::string  &SQL_QUERY);

        int UpdateRowInTable(const std::string &NameTable, DatabaseValues Values, DatabaseValues Parameters);
        // Method of make string to upper
        std::string to_upper(const std::string &sentence)
        {
            std::string new_sentence = "";
            for (int i = 0; i < sentence.length(); i++)
            {
                char ch = sentence[i];
                // cout << ch << endl;
                ch = toupper(ch);
                new_sentence += ch;
            }
            return new_sentence;
        }

    private:
        int GetArraySize(const std::string &NameTable,const std::string &NameColumn);

        static int callback(void *data, int argc, char **argv, char **azColName)
        {
            int i;
            fprintf(stderr, "%s: ", (const char *)data);

            for (i = 0; i < argc; i++)
            {
                printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
            }

            printf("\n");
            return 0;
        }
    };
}

#endif