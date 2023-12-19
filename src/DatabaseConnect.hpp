/*  The MIT License (MIT)
    ============================================================================

    ██████╗ ███████╗███████╗██████╗ ███████╗ ██████╗ ██████╗  ██████╗ ███████╗
    ██╔══██╗██╔════╝██╔════╝██╔══██╗██╔════╝██╔═══██╗██╔══██╗██╔════╝ ██╔════╝
    ██║  ██║█████╗  █████╗  ██████╔╝█████╗  ██║   ██║██████╔╝██║  ███╗█████╗
    ██║  ██║██╔══╝  ██╔══╝  ██╔═══╝ ██╔══╝  ██║   ██║██╔══██╗██║   ██║██╔══╝
    ██████╔╝███████╗███████╗██║     ██║     ╚██████╔╝██║  ██║╚██████╔╝███████╗
    ╚═════╝ ╚══════╝╚══════╝╚═╝     ╚═╝      ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝

    ████████╗ ██████╗  ██████╗ ██╗     ███████╗███████╗████████╗
    ╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝██╔════╝╚══██╔══╝
       ██║   ██║   ██║██║   ██║██║     ███████╗█████╗     ██║
       ██║   ██║   ██║██║   ██║██║     ╚════██║██╔══╝     ██║
       ██║   ╚██████╔╝╚██████╔╝███████╗███████║███████╗   ██║
       ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚══════╝   ╚═╝

    ============================================================================
    Copyright (c) 2023 DeepForge Technology
    ============================================================================
    Organization: DeepForge Technology
    ============================================================================
    Author: Blackflame576
    ============================================================================
    Created: 4 Juny 2023
    ============================================================================
*/
#ifndef DATABASECONNECT_HPP_
#define DATABASECONNECT_HPP_
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "sqlite/sqlite3.h"
#include <map>
#include <filesystem>

using namespace std;

namespace DB
{

    class Database
    {
    public:
        int ArraySize;
        sqlite3 *db;
        sqlite3_stmt *statement;
        int RESULT_SQL;
        string SQL_COMMAND;
        string AnswerDB;
        string DefaultDatabesePath = std::filesystem::current_path().generic_string() + "/DB/AppInstaller.db";
        void open(string *DB_Path = nullptr)
        {
            RESULT_SQL = sqlite3_open(DB_Path != nullptr ? DB_Path->c_str() : DefaultDatabesePath.c_str(), &db);

            // if result of open database != SQLITE_OK, that send error
            if (RESULT_SQL != SQLITE_OK)
            {
                throw runtime_error("Failed to connect to database");
            }
        }
        // Database();
        ~Database()
        {
            sqlite3_close(db);
        }
        int CreateTable(string NameTable, map<string, string> Columns);
        int InsertValuesToTable(string NameTable, map<string, string> Fields);
        int ExistNameAppInTable(string NameTable, string NameApp);
        string GetValueFromDB(string NameTable, string NameApp, string NameColumn);
        string GetVersionFromDB(string NameTable, string Channel, string NameColumn, string Architecture);
        map<string, string> GetAllVersionsFromDB(string NameTable, string NameColumn, string Architecture);
        string GetLatestVersion(string NameTable, string Channel, string NameColumn, string Architecture);
        string GetApplicationURL(string NameTable, string Channel, string NameColumn, string Architecture, string Version);
        map<string, string> GetAllValuesFromDB(string NameTable, string NameColumn);
        map<string, string> GetDevPackFromDB(string NameTable, string NameColumn);
        int InsertApplicationsToTable(string NameTable, string NameApp, string WindowsCommand, string macOSCommand, string LinuxCommand);
        int RemoveApplicationFromTable(string NameTable, string NameApp);
        int AddApplications(string Tables[]);
        int RemoveApplications(string Tables[]);
        int InsertLogInformationToTable(string NameTable, string Architecture, string OS_NAME, string Channel, string FunctionName, string LogText);
        // Method of make string to upper
        string to_upper(string sentence)
        {
            string new_sentence = "";
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
        int GetArraySize(string NameTable, string NameColumn);
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