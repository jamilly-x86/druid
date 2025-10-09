#include <gtest/gtest.h>

import druid.core.commandlineparser;

namespace
{
	constexpr auto Name = "help";
	constexpr auto ShortName = "h";
	constexpr auto Description = "Test Help Command for CommandLineOption Class";
	constexpr auto DefaultValue = "Input.txt";
	constexpr auto ValueName = "FILE";
	constexpr auto Required = true;
	constexpr auto ValueRequired = true;
	constexpr auto DefaultApplicationDescription{"Command line parser"};
}

// CommandLineOption Unit tests
TEST(CommandLineOption, constructorWithNoShortName)
{
	const druid::core::CommandLineOption option(Name);

	EXPECT_EQ(option.get_name(), Name);
	EXPECT_EQ(option.get_short_name(), "");
}

TEST(CommandLineOption, constructorWithShortName)
{
	const druid::core::CommandLineOption option(Name, ShortName);

	EXPECT_EQ(option.get_name(), Name);
	EXPECT_EQ(option.get_short_name(), ShortName);
}

TEST(CommandLineOption, setDescription)
{
	druid::core::CommandLineOption option(Name, ShortName);
	option.set_description(Description);

	EXPECT_EQ(option.get_description(), Description);
}

TEST(CommandLineOption, setDefaultValue)
{
	druid::core::CommandLineOption option(Name, ShortName);
	option.set_default_value(DefaultValue);

	EXPECT_EQ(option.get_default_value(), DefaultValue);
}

TEST(CommandLineOption, setValueName)
{
	druid::core::CommandLineOption option(Name, ShortName);
	option.set_value_name(ValueName);

	EXPECT_EQ(option.get_value_name(), ValueName);
}

TEST(CommandLineOption, setRequired)
{
	druid::core::CommandLineOption option(Name, ShortName);
	option.set_required(Required);

	EXPECT_EQ(option.get_required(), Required);
}

TEST(CommandLineOption, setValueRequired)
{
	druid::core::CommandLineOption option(Name, ShortName);
	option.set_valued_required(ValueRequired);

	EXPECT_EQ(option.get_valued_required(), ValueRequired);
}

// CommandLineParser Unit Tests
TEST(CommandLineParser, parserConstructorWithDescription)
{
	druid::core::CommandLineParser parser(DefaultApplicationDescription);

	const auto help = parser.help_text();
	EXPECT_TRUE(help.find(DefaultApplicationDescription) != std::string::npos);
}

TEST(CommandLineParser, addSingleOption)
{
	druid::core::CommandLineParser parser(DefaultApplicationDescription);
	parser.add_option(druid::core::CommandLineOption(Name, ShortName));

	EXPECT_FALSE(parser.has_error());
}





