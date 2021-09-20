#pragma once

#include <filesystem>
#include <string_view>

inline bool IsD2KPathExist(const std::string_view path) {
	return std::filesystem::exists(std::move(std::filesystem::path(path)));
}
