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
// Importing a Header File
#include <Database/DatabaseConnect.hpp>

// using namespace std;
// using namespace DB;

int DB::Database::CreateTable(const std::string &NameTable, std::map<std::string, std::string> Columns)
{
    /* The above code is creating a SQL command to create a table in a database if it does not already
    exist. It is using the variables "NameTable" and "Columns" to dynamically generate the table
    name and column names and types. The code iterates through the "Columns" map and appends each
    column name and type to the SQL command string. If it is not the last column, a comma is added
    to separate the columns. Finally, the closing parentheses are added to complete the SQL command. */
    SQL_COMMAND = "CREATE TABLE IF NOT EXISTS " + NameTable + "(\n";
    for (int i = 1; const auto &element : Columns)
    {
        SQL_COMMAND = SQL_COMMAND + element.first + " " + to_upper(element.second);
        if (i != Columns.size())
        {
            SQL_COMMAND += ",";
        }
        i++;
    }
    SQL_COMMAND += ")";
    int RESULT_SQL = sqlite3_exec(db, SQL_COMMAND.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
        throw std::runtime_error("Error in CREATE TABLE command");
    return 0;
}
int DB::Database::InsertValuesToTable(const std::string &NameTable, std::map<std::string, std::string> Fields)
{
    /* The bellow code is constructing an SQL INSERT command. It takes a table name (stored in the
    variable NameTable) and a map of fields (stored in the variable Fields) as input. */
    SQL_COMMAND = "INSERT INTO 'main'.'" + NameTable + "' ";
    std::string Columns = "(";
    std::string Values = "(";
    for (int i = 1; const auto &element : Fields)
    {
        Columns = Columns + "'" + element.first + "'";
        Values = Values + "'" + element.second + "'";
        if (i != Fields.size())
        {
            Columns += ",";
            Values += ",";
        }
        i++;
    }
    Columns += ")";
    Values += ")";
    SQL_COMMAND = SQL_COMMAND + Columns + " VALUES " + Values + ";";
    int RESULT_SQL = sqlite3_exec(db, SQL_COMMAND.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
        throw std::runtime_error("Error in INSERT command");
    return 0;
}
int DB::Database::ExistNameAppInTable(const std::string &NameTable, const std::string &NameApp)
{
    // Create SQL statement
    SQL_COMMAND = "SELECT Name FROM " + NameTable + " WHERE Name='" + NameApp + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        return -1;
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return 0;
}
std::string DB::Database::GetValueFromDB(const std::string &NameTable, const std::string &NameApp, const std::string &NameColumn)
{
    std::string AnswerDB;
    // Create SQL statement
    SQL_COMMAND = "SELECT " + NameColumn + " FROM " + NameTable + " WHERE Name='" + NameApp + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw std::runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        AnswerDB = (std::string(reinterpret_cast<const char *>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return AnswerDB;
}

std::string DB::Database::GetVersionFromDB(const std::string &NameTable, const std::string &Channel, const std::string &NameColumn, const std::string &Architecture)
{
    std::string AnswerDB;
    // Create SQL statement
    SQL_COMMAND = "SELECT " + NameColumn + " FROM " + NameTable + " WHERE Channel='" + Channel + "' AND Architecture='" + Architecture + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw std::runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        AnswerDB = (std::string(reinterpret_cast<const char *>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return AnswerDB;
}

std::string DB::Database::GetApplicationURL(const std::string &NameTable, const std::string &Channel, const std::string &NameColumn, const std::string &Architecture, const std::string &Version)
{
    std::string AnswerDB;
    // Create SQL statement
    SQL_COMMAND = "SELECT " + NameColumn + " FROM " + NameTable + " WHERE Channel='" + Channel + "' AND Architecture='" + Architecture + "' AND Version='" + Version + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw std::runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        // printf("%s\n", sqlite3_column_text(statement, 0));
        AnswerDB = (std::string(reinterpret_cast<const char *>(
            sqlite3_column_text(statement, 0))));
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    return AnswerDB;
}

std::map<std::string, std::string> DB::Database::GetAllValuesFromDB(const std::string &NameTable, const std::string &NameColumn)
{
    std::map<std::string, std::string> WriteMap;

    // Create SQL statement
    SQL_COMMAND = "SELECT Name," + NameColumn + " FROM " + NameTable;
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw std::runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        std::string Key = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 0)));
        std::string Value = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));
        if (Value != "Not Found")
        {
            WriteMap.insert(std::pair<std::string, std::string>(Key, Value));
        }
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

std::map<std::string, std::string> DB::Database::GetAllVersionsFromDB(const std::string &NameTable, const std::string &Architecture)
{
    std::map<std::string, std::string> WriteMap;
    // Create SQL statement
    // SQL_COMMAND = "SELECT Channel,Version FROM '" + NameTable + "' WHERE Architecture='" + Architecture + "'";
    SQL_COMMAND = "SELECT Channel,Version FROM '" + NameTable + "' WHERE Architecture='" + Architecture + "'";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw std::runtime_error("Not Found");
    }
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        std::string Key = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 0)));
        std::string Value = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));
        if (Value != "Empty")
        {
            WriteMap.insert(std::pair<std::string, std::string>(Key, Value));
        }
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

std::string DB::Database::GetLatestVersion(const std::string &NameTable, const std::string &Channel, const std::string &NameColumn, const std::string &Architecture)
{
    std::string AnswerDB;
    int tableCount = GetArraySize(NameTable, NameColumn);
    if (tableCount > 0)
    {
        // Create SQL statement
        SQL_COMMAND = "SELECT max(" + NameColumn + ") FROM " + NameTable + " WHERE Channel='" + Channel + "' AND Architecture='" + Architecture + "'";
        // Execute SQL statement
        int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
        // if result of execute sql statement != SQLITE_OK, that send error
        if (RESULT_SQL != SQLITE_OK)
        {
            throw std::runtime_error("Not Found");
        }
        // Loop through the results, a row at a time.
        while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
        {
            AnswerDB = (std::string(reinterpret_cast<const char *>(
                sqlite3_column_text(statement, 0))));
        }
        // Free the statement when done.
        sqlite3_finalize(statement);
        // return Names,Commands;
        return AnswerDB;
    }
    else
    {
        return "";
    }
}

std::map<std::string, std::string> DB::Database::GetDevPackFromDB(const std::string &NameTable, const std::string &NameColumn)
{
    std::map<std::string, std::string> WriteMap;
    // Create SQL statement
    SQL_COMMAND = "SELECT Number," + NameColumn + " FROM " + NameTable;
    // SQL_COMMAND = "SELECT Number,Language FROM DevelopmentPacks";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw std::runtime_error("Not Found");
    }
    int i = 0;
    // Loop through the results, a row at a time.
    while ((RESULT_SQL = sqlite3_step(statement)) == SQLITE_ROW)
    {
        std::string Key = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 0)));
        std::string Value = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));
        if (Value != "Not Found")
        {
            WriteMap.insert(std::pair<std::string, std::string>(Key, Value));
        }
        i++;
    }
    // Free the statement when done.
    sqlite3_finalize(statement);
    // return Names,Commands;
    return WriteMap;
}

int DB::Database::GetArraySize(const std::string &NameTable, const std::string &NameColumn)
{
    // Create SQL statement
    SQL_COMMAND = "SELECT count(" + NameColumn + ") " + " FROM " + NameTable;
    // SQL_COMMAND = "SELECT count(Windows) FROM Applications";
    // Execute SQL statement
    int RESULT_SQL = sqlite3_prepare_v2(db, SQL_COMMAND.c_str(), SQL_COMMAND.length(), &statement, nullptr);
    // if result of execute sql statement != SQLITE_OK, that send error
    if (RESULT_SQL != SQLITE_OK)
    {
        throw std::runtime_error("Not Found");
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

int DB::Database::InsertApplicationsToTable(const std::string &NameTable, const std::string &NameApp, const std::string &WindowsCommand, const std::string &macOSCommand, const std::string &LinuxCommand)
{
    /* The bellow code is constructing an SQL INSERT command to insert data into a table. The command is
    dynamically generated based on the values of variables `NameTable`, `NameApp`, `WindowsCommand`,
    `macOSCommand`, and `LinuxCommand`. The command will insert a new row into the specified table
    with the values provided for the columns 'Name', 'Windows', 'macOS', and 'Linux'. */
    SQL_COMMAND = "INSERT INTO 'main'.'" + NameTable + "' ('Name', 'Windows', 'macOS', 'Linux') VALUES ('" + NameApp + "', '" + WindowsCommand + "', '" + macOSCommand + "', '" + LinuxCommand + "');";
    int RESULT_SQL = sqlite3_exec(db, SQL_COMMAND.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
        throw std::runtime_error("Error in INSERT command");
    return 0;
}

int DB::Database::RemoveApplicationFromTable(const std::string &NameTable, const std::string &NameApp)
{
    /* The bellow code is constructing a SQL command to delete a row from a table in a database. The
    table name is stored in the variable "NameTable" and the row to be deleted is specified by the
    value of the variable "NameApp". The constructed SQL command will delete the row where the
    "Name" column matches the value of "NameApp". */
    SQL_COMMAND = "DELETE FROM " + NameTable + " WHERE Name='" + NameApp + "'";
    int RESULT_SQL = sqlite3_exec(db, SQL_COMMAND.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
        throw std::runtime_error("Error in DELETE command");
    return 0;
}

int DB::Database::AddApplications(const std::string Tables[])
{
    std::string NameApp;
    std::string Windows_Command;
    std::string macOS_Command;
    std::string Linux_Command;
    int RESULT_COMMAND;

    /* The bellow code is checking if the size of the "Tables" vector is greater than or equal to 1. If
    it is, it prompts the user to enter the name, Windows command, macOS command, and Linux command
    for an application. Then, it iterates through each element in the "Tables" vector and calls the
    "InsertApplicationsToTable" function to insert the application information into each table. If
    the insertion is successful (RESULT_COMMAND is 0), it prints a success message indicating that
    the application was added to the corresponding table. */
    if (Tables->size() >= 1)
    {
        std::cout << "Name:";
        std::getline(std::cin, NameApp);
        std::cout << "Windows:";
        std::getline(std::cin, Windows_Command);
        std::cout << "macOS:";
        std::getline(std::cin, macOS_Command);
        std::cout << "Linux:";
        std::getline(std::cin, Linux_Command);
        for (int i = 0; i < Tables->size(); i++)
        {
            RESULT_COMMAND = InsertApplicationsToTable(Tables[i], NameApp, Windows_Command, macOS_Command, Linux_Command);
            if (RESULT_COMMAND == 0)
            {
                std::cout << NameApp << " successfully added to " << Tables[i] << std::endl;
            }
        }
    }
    else
    {
        throw std::logic_error("Array is empty");
    }
    return 0;
}

int DB::Database::RemoveApplications(const std::string Tables[])
{
    std::string NameApp;
    std::string Windows_Command;
    std::string macOS_Command;
    std::string Linux_Command;
    int RESULT_COMMAND;

    /* The bellow code is checking if the size of the "Tables" object is greater than or equal to 1. If
    it is, it prompts the user to enter the name of an application, as well as commands for Windows,
    macOS, and Linux. */
    if (Tables->size() >= 1)
    {
        std::cout << "Name:";
        std::getline(std::cin, NameApp);
        std::cout << "Windows:";
        std::getline(std::cin, Windows_Command);
        std::cout << "macOS:";
        std::getline(std::cin, macOS_Command);
        std::cout << "Linux:";
        std::getline(std::cin, Linux_Command);

        for (int i = 0; i < Tables->size(); i++)
        {
            RESULT_COMMAND = RemoveApplicationFromTable(Tables[i], NameApp);
            if (RESULT_COMMAND == 0)
                std::cout << NameApp << " successfully removed to " << Tables[i] << std::endl;
        }
    }
    return 0;
}

int DB::Database::InsertLogInformationToTable(const std::string &NameTable, const std::string &Architecture, const std::string &OS_NAME, const std::string &Channel, const std::string &FunctionName, const std::string &LogText)
{
    /* The bellow code is constructing an SQL INSERT statement. It is creating a command to insert data
    into a table named 'NameTable' in the 'main' database. The data being inserted includes values
    for the columns 'Architecture', 'Channel', 'LogText', 'OS_NAME', and 'FunctionName'. The values
    for these columns are being passed as variables in the code. */
    SQL_COMMAND = "INSERT INTO 'main'.'" + NameTable + "' ('Architecture', 'Channel', 'LogText', 'OS_NAME','FunctionName') VALUES ('" + Architecture + "', '" + Channel + "', '" + LogText + "', '" + OS_NAME + "', '" + FunctionName + "');";
    int RESULT_SQL = sqlite3_exec(db, SQL_COMMAND.c_str(), callback, NULL, NULL);
    if (RESULT_SQL != SQLITE_OK)
        throw std::runtime_error("Error in INSERT command");
    return 0;
}