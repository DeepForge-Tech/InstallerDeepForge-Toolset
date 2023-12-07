#include "Logger.hpp"

void Logger::WriteDebug(string LogText)
{   
    string text = DebugStr + "::[" + GetTime() + "]" + ":::" + LogText;
    WriteFile(LogPath,text);
}

void Logger::WriteError(string LogText)
{
    string text = ErrorStr + "::[" + GetTime() + "]" + ":::" + LogText;
    WriteFile(LogPath,text);
}

void Logger::WriteInfo(string LogText)
{
    string text = InfoStr + "::[" + GetTime() + "]" + ":::" + LogText;
    WriteFile(LogPath,text);
}
void Logger::WriteWarning(string LogText)
{
    string text = WarningStr + "::[" + GetTime() + "]" + ":::" + LogText;
    WriteFile(LogPath,text);
}
void Logger::WriteSuccess(string LogText)
{
    string text = SuccessStr + "::[" + GetTime() + "]" + ":::" + LogText;
    WriteFile(LogPath,text);
}

void Logger::LogDebug(string LogText)
{
    string text = DebugStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
}

void Logger::LogError(string LogText)
{
    string text = ErrorStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
}

void Logger::LogInfo(string LogText)
{
    string text = InfoStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
}

void Logger::LogWarning(string LogText)
{
    string text = WarningStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
}

void Logger::LogSuccess(string LogText)
{
    string text = SuccessStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
}

void Logger::WRITE_AND_LOG_DEBUG(string LogText)
{
    string text = DebugStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
    WriteFile(LogPath,text);
}

void Logger::WRITE_AND_LOG_ERROR(string LogText)
{
    string text = ErrorStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
    WriteFile(LogPath,text);
}

void Logger::WRITE_AND_LOG_INFO(string LogText)
{
    string text = InfoStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
    WriteFile(LogPath,text);
}

void Logger::WRITE_AND_LOG_SUCCESS(string LogText)
{
    string text = SuccessStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
    WriteFile(LogPath,text);
}

void Logger::WRITE_AND_LOG_WARNING(string LogText)
{
    string text = WarningStr + "::[" + GetTime() + "]" + ":::" + LogText;
    cerr << text << endl;
    WriteFile(LogPath,text);
}

void Logger::SendError(string Architecture,string Channel,string OS_NAME,string FunctionName,string LogText)
{
    string data = R"({
        "Entries":
        [
        {
        "Architecture":")" + Architecture + R"(",
        "Channel":")" + Channel + R"(",
        "OS_NAME":")" + OS_NAME + R"(",
        "FunctionName":")" + FunctionName + R"(",
        "LogText":")" + LogText + R"(",
        }
        ]
    })";
    UDP_CLIENT.SEND_JSON_ARRAY(data);
}