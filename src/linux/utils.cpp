#include "linux/utils.h"
#include <sys/stat.h>

bool Utils::getFileExists(const std::string &file_path) {
    struct stat buffer;
    return (stat(file_path.c_str(), &buffer) == 0);
}