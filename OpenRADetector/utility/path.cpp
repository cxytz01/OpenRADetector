#include "path.h"

#include <filesystem>
#include <string_view>

bool IsD2KPathExist(const std::string_view path) {
	std::filesystem::path d2kpath(path);
	return std::filesystem::exists(d2kpath);
}