module;

#include <boost/program_options.hpp>
#include <optional>
#include <string>
#include <vector>

export module druid.core.commandlineparser;

export namespace druid::core
{
	namespace po = boost::program_options;

	class CommandLineParser
	{
	public:
		explicit CommandLineParser(std::string programName, std::optional<std::string> programDescription)
			: optionsDescription("Options"), programName(std::move(programName)), programDescription(std::move(programDescription))
		{
			// should always have help option
			this->optionsDescription.add_options()("help,h", "Display this help message");
		}

		CommandLineParser& addFlag(const std::string& name, const std::string& description)
		{
			this->optionsDescription.add_options()(name.c_str(), description.c_str());
			return *this;
		}

		template <typename T>
		CommandLineParser& addOption(const std::string& name, const std::string& description)
		{
			this->optionsDescription.add_options()(name.c_str(), po::value<T>(), description.c_str());
			return *this;
		}

		template <typename T>
		CommandLineParser& addOption(const std::string& name, const T& defaultValue, const std::string& description)
		{
			this->optionsDescription.add_options()(name.c_str(), po::value<T>()->default_value(defaultValue), description.c_str());
			return *this;
		}

		CommandLineParser& addPositional(const std::string& name, int maxCount = -1)
		{
			this->positionalDescription.add(name.c_str(), maxCount);
			this->positionalNames.emplace_back(name);
			return *this;
		}

		void setParsed(bool parsed)
		{
			if(this->parsed != parsed)
			{
				this->parsed = parsed;
			}
		}

		[[nodiscard]] bool getParsed() const
		{
			return this->parsed;
		}

		[[nodiscard]] bool parse(int argc, char* argv[])
		{
			try
			{
				po::store(po::command_line_parser(argc, argv).options(this->optionsDescription).positional(this->positionalDescription).run(),
						  this->variablesMap);
				po::notify(this->variablesMap);

				if(this->variablesMap.count("help") == true)
				{
					// this->printHelp();
					return this->getParsed();
				}

				this->setParsed(true);
				return this->getParsed();
			}
			catch(const po::error& e)
			{
				this->errorMessage = e.what();
				return this->getParsed();
			}
		}

	private:
		po::options_description optionsDescription;
		po::variables_map variablesMap;
		po::positional_options_description positionalDescription;
		std::vector<std::string> positionalNames;
		std::string programName;
		std::optional<std::string> programDescription;
		std::string errorMessage;
		bool parsed{false};
	};
}