#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>

std::string currentTime(std::chrono::time_point<std::chrono::system_clock> now);

bool endsWith(std::string const &originalString, std::string const &ending);

static auto vector_getter = [](void *vec, int idx, const char **out_text)
{
    auto &vector = *static_cast<std::vector<std::string> *>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size()))
    {
        return false;
    }
    *out_text = vector.at(idx).c_str();
    return true;
};
