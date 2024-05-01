/*  GNU GENERAL PUBLIC LICENSE
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
    Copyright (c) 2024 DeepForge Technology
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
#include <cstdio>
#include <cstdlib>
#include <sqlite3/sqlite3.h>
#include <map>
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
        int CreateTable(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                        std::map<std::string, std::string> Columns);

        int InsertValuesToTable(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                std::map<std::string, std::string> Fields);

        int ExistNameAppInTable(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameApp);

        std::string GetValueFromDB(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable,
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameApp, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameColumn);

        std::string GetVersionFromDB(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> Channel, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameColumn, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> Architecture);

        std::map<std::string, std::string> GetAllVersionsFromDB(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                                                std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameColumn, 
                                                                std::basic_string<char, std::char_traits<char>, std::allocator<char>> Architecture);

        std::string GetLatestVersion(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> Channel, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameColumn, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> Architecture);

        std::string GetApplicationURL(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> Channel, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameColumn, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> Architecture, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> Version);

        std::map<std::string, std::string> GetAllValuesFromDB(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                                            std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameColumn);

        std::map<std::string, std::string> GetDevPackFromDB(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                                            std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameColumn);

        int InsertApplicationsToTable(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameApp, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> WindowsCommand, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> macOSCommand, 
                                    std::basic_string<char, std::char_traits<char>, std::allocator<char>> LinuxCommand);

        int RemoveApplicationFromTable(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                        std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameApp);

        int AddApplications(std::string Tables[]);

        int RemoveApplications(std::string Tables[]);

        int InsertLogInformationToTable(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                                        std::basic_string<char, std::char_traits<char>, std::allocator<char>> Architecture, 
                                        std::basic_string<char, std::char_traits<char>, std::allocator<char>> OS_NAME, 
                                        std::basic_string<char, std::char_traits<char>, std::allocator<char>> Channel, 
                                        std::basic_string<char, std::char_traits<char>, std::allocator<char>> FunctionName, 
                                        std::basic_string<char, std::char_traits<char>, std::allocator<char>> LogText);
        // Method of make string to upper
        std::string to_upper(std::basic_string<char, std::char_traits<char>, std::allocator<char>> sentence)
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
        int GetArraySize(std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameTable, 
                        std::basic_string<char, std::char_traits<char>, std::allocator<char>> NameColumn);

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