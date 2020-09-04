#include <fstream>
#include <json/json.h>


Json::Value load_keys(std::string conf_filename)
{
    Json::Value keys_json;
    std::ifstream file_handle(__KEYS_PATH);
    if (!file_handle.is_open())
    {
        std::cout << "failed to load keys json" << std::endl;
        exit(1);
    };

    file_handle >> keys_json;

    return keys_json;
}
