#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>

namespace druid::core
{
	auto ReadFile(const std::filesystem::path& x) -> std::vector<std::byte>;
}