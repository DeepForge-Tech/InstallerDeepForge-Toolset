#if defined(__linux__) || __APPLE__
#include "Client_UNIX.hpp"
using namespace UNIX;
#elif _WIN32
#include "Client_POSIX.hpp"
using namespace POSIX;
#endif
#include <filesystem>

int main()
{
    Client client;
    // char *data = client.OpenJSON("./toJSON.json");
    string ProjectDir = std::filesystem::current_path().generic_string();
    string JSON_Path = ProjectDir + "/toJson.json";
    client.SendJSON(JSON_Path);
    return 0;
}