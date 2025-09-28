module;

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>

export module druid.core.file;

namespace druid::core
{
	export auto ReadFile(const std::filesystem::path& x) -> std::vector<std::byte>
	{
		std::vector<std::byte> v;
		std::ifstream ifs{x, std::ios::binary | std::ios::ate};

		if(ifs.is_open() == false)
		{
			return v;
		}

		v.resize(ifs.tellg());
		ifs.seekg(0, std::ios::beg);

		// NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
		ifs.read(reinterpret_cast<char*>(v.data()), static_cast<std::streamsize>(v.size()));
		return v;
	}
}
