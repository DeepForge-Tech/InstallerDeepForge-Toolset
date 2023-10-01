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
// Importing a Header File
#include "DatabaseConnect.hpp"

using namespace std;
using namespace DB;

string Database::GetValueFromDB(string NameTable, string NameApp, string NameColumn)
{
    // ArraySize = GetArraySize("Applications",NameColumn);
    // string* AnswerDB = new string[ArraySize];
    string AnswerDB;
    // Create SQL statement
    SQL_COMMAND = "SELECT " + NameColumn + " FROM " + NameTable + " WHERE Name='" + NameApp + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        AnswerDB = (string(reinterpret_cast<const char *>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return AnswerDB;
}

string Database::GetVersionFromDB(string NameTable,string Channel,string NameColumn,string Architecture) 
{
    string AnswerDB;
    // Create SQL statement
    SQL_COMMAND = "SELECT " + NameColumn + " FROM " + NameTable + " WHERE Channel='" + Channel + "' AND Architecture='" + Architecture + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        AnswerDB = (string(reinterpret_cast<const char*>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return AnswerDB;
}

string Database::GetApplicationURL(string NameTable,string Channel,string NameColumn,string Architecture,string Version)
{
    string AnswerDB;
    // Create SQL statement
    SQL_COMMAND = "SELECT " + NameColumn + " FROM " + NameTable + " WHERE Channel='" + Channel + "' AND Architecture='" + Architecture + "' AND Version='" + Version + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        AnswerDB = (string(reinterpret_cast<const char*>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return AnswerDB;
}

map<string, string> Database::GetAllValuesFromDB(string NameTable, string NameColumn)
{
    map<string, string> WriteMap;

    // Create SQL statement
    SQL_COMMAND = "SELECT Name," + NameColumn + " FROM " + NameTable;
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        string Key = string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 0)));
        string Value = string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));
        if (Value != "Not Found")
        {
            WriteMap.insert(pair<string, string>(Key, Value));
        }
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

map<string,string> Database::GetAllVersionsFromDB(string NameTable,string NameColumn,string Architecture)
{
    map<string, string> WriteMap;
    // Create SQL statement
    SQL_COMMAND = "SELECT Channel,Version FROM " + NameTable + " WHERE Architecture='" + Architecture + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        string Key = string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 0)));
        string Value = string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));
        if (Value != "Empty")
        {
            WriteMap.insert(pair<string, string>(Key, Value));
        }
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

string Database::GetLatestVersion(string NameTable,string Channel,string NameColumn,string Architecture)
{
    string AnswerDB;
    // cout << "fffef" <<endl;
    // Create SQL statement
    SQL_COMMAND = "SELECT max(" + NameColumn + ") FROM " + NameTable + " WHERE Channel='" + Channel + "' AND Architecture='" + Architecture + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        AnswerDB = (string(reinterpret_cast<const char*>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return AnswerDB;
}

map<string, string> Database::GetDevPackFromDB(string NameTable, string NameColumn)
{
    map<string, string> WriteMap;

    // Create SQL statement
    SQL_COMMAND = "SELECT Number," + NameColumn + " FROM " + NameTable;
    // SQL_COMMAND = "SELECT Number,Language FROM DevelopmentPacks";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw runtime_error("Not Found");
    }
    int i = 0;
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        string Key = string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 0)));
        string Value = string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));
        if (Value != "Not Found")
        {
            WriteMap.insert(pair<string, string>(Key, Value));
        }
        i++;
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

int Database::GetArraySize(string NameTable, string NameColumn)
{
    // Create SQL statement
    SQL_COMMAND = "SELECT count(" + NameColumn + ") " + " FROM " + NameTable;
    // SQL_COMMAND = "SELECT count(Windows) FROM Applications";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        ArraySize = sqlite3_column_int(statement, 0);
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return ArraySize;
}

int Database::InsertApplicationsToTable(string NameTable, string NameApp, string WindowsCommand, string macOSCommand, string LinuxCommand)
{
    SQL_COMMAND = "INSERT INTO 'main'.'" + NameTable + "' ('Name', 'Windows', 'macOS', 'Linux') VALUES ('" + NameApp + "', '" + WindowsCommand + "', '" + macOSCommand + "', '" + LinuxCommand + "');";
    int RESULT_SQL = sqlite3_exec(db, SQL_COMMAND.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
        throw runtime_error("Error in INSERT command");
    return 0;
}

int Database::RemoveApplicationsFromTable(string NameTable, string NameApp)
{
    SQL_COMMAND = "DELETE FROM " + NameTable + " WHERE Name='" + NameApp + "'";
    int RESULT_SQL = sqlite3_exec(db, SQL_COMMAND.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
        throw runtime_error("Error in DELETE command");
    return 0;
}

int Database::AddApplications(string Tables[])
{
    string NameApp;
    string Windows_Command;
    string macOS_Command;
    string Linux_Command;
    int RESULT_COMMAND;

    if (Tables->size() >= 1)
    {
        cout << "Name:";
        getline(cin, NameApp);
        cout << "Windows:";
        getline(cin, Windows_Command);
        cout << "macOS:";
        getline(cin, macOS_Command);
        cout << "Linux:";
        getline(cin, Linux_Command);
        for (int i = 0; i < Tables->size(); i++)
        {
            RESULT_COMMAND = InsertApplicationsToTable(Tables[i], NameApp, Windows_Command, macOS_Command, Linux_Command);
            if (RESULT_COMMAND == 0)
            {
                cout << NameApp << " successfully added to " << Tables[i] << endl;
            }
        }
    }
    else
    {
        throw logic_error("Array is empty");
    }
    return 0;
}

int Database::RemoveApplications(string Tables[])
{
    string NameApp;
    string Windows_Command;
    string macOS_Command;
    string Linux_Command;
    int RESULT_COMMAND;

    if (Tables->size() >= 1)
    {
        cout << "Name:";
        getline(cin, NameApp);
        cout << "Windows:";
        getline(cin, Windows_Command);
        cout << "macOS:";
        getline(cin, macOS_Command);
        cout << "Linux:";
        getline(cin, Linux_Command);

        for (int i = 0; i < Tables->size(); i++)
        {
            RESULT_COMMAND = RemoveApplicationsFromTable(Tables[i], NameApp);
            if (RESULT_COMMAND == 0)
                cout << NameApp << " successfully added to " << Tables[i] << endl;
        }
    }
    return 0;
}