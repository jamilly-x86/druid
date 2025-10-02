#include <gtest/gtest.h>
#include <string>
#include <vector>

import druid.core.commandlineparser;

namespace dc = druid::core;

namespace
{
	class ArgvBuilder
	{
	public:
		auto add(const std::string& arg) -> ArgvBuilder&
		{
			this->args.emplace_back(std::move(arg));
			return *this;
		}

		auto argc() const -> int
		{
			return static_cast<int>(this->args.size());
		}

		auto argv() -> char**
		{
			this->ptrs.clear();
			for(auto& arg : this->args)
			{
				this->ptrs.emplace_back(arg.data());
			}
			return this->ptrs.data();
		}

	private:
		std::vector<std::string> args;
		std::vector<char*> ptrs;
	};
}

TEST(CommandLineParser, constructorNoDescription)
{
	const dc::CommandLineParser parser("test", std::nullopt);
}

TEST(CommandLineParser, constructorWithDescription)
{
	const dc::CommandLineParser parser("test", "test application");
}

// TEST(CommandLineParser, flagsAreFalseByDefault)
// {
// 	dc::CommandLineParser parser("test", std::nullopt);
// 	parser.addFlag("verbose,v", "Verbose Output").addFlag("debug,d", "Debug mode");
//
// 	ArgvBuilder args;
// 	args.add("test");
// }