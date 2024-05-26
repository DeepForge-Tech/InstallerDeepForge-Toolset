#ifndef DATABASEAPI_HPP_
#define DATABASEAPI_HPP_
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sqlite3/sqlite3.h>
#include <unordered_map>
#include <filesystem>

namespace DB
{
    class Database
    {
    public:
        int ArraySize;
        sqlite3 *db;
        sqlite3_stmt *statement;
        int RESULT_SQL;
        std::string SQL_COMMAND;
        std::string AnswerDB;
        std::string DefaultDatabesePath = std::filesystem::current_path().generic_string() + "/DB/AppInstaller.db";
        void open(std::string *DB_Path = nullptr)
        {
            RESULT_SQL = sqlite3_open(DB_Path != nullptr ? DB_Path->c_str() : DefaultDatabesePath.c_str(), &db);

            // if result of open database != SQLITE_OK, that send error
            if (RESULT_SQL != SQLITE_OK)
            {
                throw std::runtime_error("Failed to connect to database");
            }
        }
        // Database();
        ~Database()
        {
            sqlite3_close(db);
        }
        int CreateTable(const std::string &NameTable, std::unordered_map<std::string, std::string> Columns);

        int InsertValuesToTable(const std::string &NameTable, std::unordered_map<std::string, std::string> Fields);

        bool ExistValueInTable(const std::string &NameTable,const std::string &NameColumn,const std::string &Value);

        std::string GetValueFromDB(const std::string &NameTable,const std::string &NameApp,const std::string &NameColumn);

        std::string GetVersionFromDB(const std::string &NameTable,const std::string &Channel,const std::string &NameColumn,const std::string &Architecture);

        std::unordered_map<std::string, std::string> GetAllVersionsFromDB(const std::string &NameTable,const std::string &Architecture);

        std::string GetLatestVersion(const std::string &NameTable,const std::string &Channel,const std::string &NameColumn,const std::string &Architecture);

        std::string GetApplicationURL(const std::string &NameTable,const std::string &Channel,const std::string &NameColumn,const std::string &Architecture,const std::string &Version);

        std::unordered_map<std::string, std::string> GetAllValuesFromDB(const std::string &NameTable,const std::string &NameColumn);

        std::unordered_map<std::string, std::string> GetDevPackFromDB(const std::string &NameTable,const std::string &NameColumn);

        int InsertApplicationsToTable(const std::string &NameTable,const std::string &NameApp,const std::string &WindowsCommand,const std::string &macOSCommand,const std::string &LinuxCommand);

        int RemoveApplicationFromTable(const std::string &NameTable,const std::string &NameApp);

        int AddApplications(const std::string Tables[]);

        int RemoveApplications(const std::string Tables[]);

        int InsertLogInformationToTable(const std::string &NameTable, const std::string &Architecture,const std::string &OS_NAME,const std::string &Channel,const std::string &FunctionName,const std::string &LogText);
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