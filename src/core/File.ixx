module;

#include <filesystem>
#include <fstream>
#include <vector>

export module druid.core.File;

export namespace druid::core
{
	/// @brief Read the entire contents of a file into a byte vector.
	///
	/// @param x Path to the file to read.
	/// @return Vector containing the file's contents as bytes, or empty vector on failure.
	auto ReadFile(const std::filesystem::path& x) -> std::vector<std::byte>
	{
		std::vector<std::byte> v;
		std::ifstream ifs{x, std::ios::binary | std::ios::ate};

		if (ifs.is_open() == false)
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
