module;

#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <vector>

export module druid.core.commandlineparser;

export namespace druid::core
{
	/// @class CommandLineOption
	/// @brief Represents a single command-line option with its configuration
	///
	/// This class encapsulates all the properties of a command-line option including
	/// ts name, description, default value, and behavioral flags.
	///
	/// @example Basic option creation
	/// @code
	/// druid::core::CommandLineOption option("verbose", "v");
	/// option.set_description("Enable verbose output");
	/// option.set_value_required(false);
	/// @endcode
	class CommandLineOption
	{
	public:
		CommandLineOption() = delete;

		/// @brief Constructs a command-line option with a long name and optional short name
		///
		/// @param name The long option name (e.g., "verbose" for --verbose)
		/// @param short_name Optional single-character short name (e.g., "v" for -v)
		///
		/// @note The name should not include the leading dashes (-- or -)
		/// @note The short name should be a single character
		///
		/// @example
		/// @code
		/// druid::core::CommandLineOption verbose("verbose", "v");  // Creates --verbose, -v
		/// druid::core::CommandLineOption output("output");         // Creates --output (no short form)
		/// @endcode
		explicit CommandLineOption(std::string name, std::string short_name = "") : name_(std::move(name)), short_name_(std::move(short_name))
		{
		}

		/// @brief Gets the long option name
		/// @return Const reference to the option name
		[[nodiscard]] auto get_name() const noexcept -> const std::string&
		{
			return name_;
		}

		/// @brief Gets the short option name
		/// @return Const reference to the short name (empty if not set)
		[[nodiscard]] auto get_short_name() const noexcept -> const std::string&
		{
			return short_name_;
		}

		/// @brief Sets the human-readable description for this option
		///
		/// The description is displayed in the help text to explain what the option does.
		///
		/// @param description The description text
		///
		/// @example
		/// @code
		/// option.set_description("Enable verbose logging with detailed output");
		/// @endcode
		auto set_description(const std::string& description) -> void
		{
			if(description_ != description)
			{
				description_ = description;
			}
		}

		/// @brief Gets the option description
		/// @return Const reference to the description text
		[[nodiscard]] auto get_description() const noexcept -> const std::string&
		{
			return description_;
		}

		/// @brief Sets the default value for this option
		///
		/// If the option is not specified on the command line, this value will be used.
		/// The default value is stored as a string and converted when retrieved via value<T>().
		///
		/// @param default_value The default value as a string
		///
		/// @note Default values are shown in the help text
		/// @note An empty string means no default value
		///
		/// @example
		/// @code
		/// option.set_default_value("4");       // For integer option
		/// option.set_default_value("out.txt"); // For string option
		/// @endcode
		auto set_default_value(const std::string& default_value) -> void
		{
			if(default_value_ != default_value)
			{
				default_value_ = default_value;
			}
		}

		/// @brief Gets the default value
		/// @return Const reference to the default value string (empty if not set)
		[[nodiscard]] auto get_default_value() const noexcept -> const std::string&
		{
			return default_value_;
		}

		/// @brief Sets the placeholder name for the option's value in help text
		///
		/// This is purely cosmetic and only affects help text display.
		///
		/// @param value_name The placeholder name (e.g., "FILE", "COUNT", "PATH")
		///
		/// @example
		/// @code
		/// option.setValueName("FILE");
		/// // Help text shows: --input <FILE>
		///
		/// option.setValueName("NUMBER");
		/// // Help text shows: --threads <NUMBER>
		/// @endcode
		auto set_value_name(const std::string& value_name) -> void
		{
			if(value_name_ != value_name)
			{
				value_name_ = value_name;
			}
		}

		/// @brief Gets the value placeholder name
		/// @return Const reference to the value name (empty if not set)
		[[nodiscard]] auto get_value_name() const noexcept -> const std::string&
		{
			return value_name_;
		}

		/// @brief Marks this option as required or optional
		///
		/// Required options must be present on the command line, otherwise
		/// parsing will fail with an error.
		///
		/// @param required True to mark as required, false for optional (default: true)
		///
		/// @note Required options are marked in the help text
		///
		/// @example
		/// @code
		/// option.set_required(true);   // Must be specified
		/// option.set_required(false);  // Optional
		/// option.set_required();       // Same as setRequired(true)
		/// @endcode
		auto set_required(bool required = true) -> void
		{
			if(required_ != required)
			{
				required_ = required;
			}
		}

		/// @brief Checks if this option is required
		/// @return True if required, false otherwise
		[[nodiscard]] auto get_required() const noexcept -> bool
		{
			return required_;
		}

		/// @brief Sets whether this option requires a value
		///
		/// Flag options (like --verbose) don't require values, while regular options
		/// (like --input file.txt) do.
		///
		/// @param value_required True if option requires a value, false for flag options (default: true)
		/// @return Reference to this object for method chaining
		///
		/// @example
		/// @code
		/// // Flag option (no value), Usage: --verbose
		/// verbose.set_valued_required(false);
		///
		/// // Regular option (requires value), Usage: --input file.txt
		/// input.set_valued_required(true);
		/// @endcode
		auto set_valued_required(bool value_required = true) -> void
		{
			if(value_required_ != value_required)
			{
				required_ = value_required;
			}
		}

		/// @brief Checks if this option requires a value
		/// @return True if value required, false for flag options
		[[nodiscard]] auto get_valued_required() const noexcept -> bool
		{
			return value_required_;
		}

	private:
		std::string name_;
		std::string short_name_;
		std::string description_;
		std::string default_value_;
		std::string value_name_;
		bool required_{false};
		bool value_required_{true};
	};

	class CommandLineParser
	{
	public:
		CommandLineParser() = delete;

		explicit CommandLineParser(std::string application_description) : application_description_(std::move(application_description))
		{
		}

		[[nodiscard]] auto error_text() const noexcept -> const std::string&
		{
			return error_text_;
		}

		[[nodiscard]] auto has_error() const noexcept -> bool
		{
			return !std::empty(error_text_);
		}

		[[nodiscard]] auto help_text() -> std::string
		{
			std::ostringstream oss;
			oss << application_description_ << "\n\n";
			oss << "Options:\n";

			// collect all unique options (avoiding duplicates from short names)
			std::set<std::string> processed_names;

			for(const auto& parsed : options_ | std::ranges::views::values)
			{
				// skip if we have already processed this option
				if(processed_names.contains(parsed.option.get_name()) == true)
				{
					continue;
				}
				processed_names.insert(parsed.option.get_name());

				const auto& opt = parsed.option;
				oss << "  --" << opt.get_name();

				if(std::empty(opt.get_short_name()) == false)
				{
					oss << ", -" << opt.get_short_name();
				}
				if(opt.get_valued_required() == true)
				{
					if(std::empty(opt.get_value_name()) == false)
					{
						oss << " <" << opt.get_value_name() << ">";
					}
					else
					{
						oss << " <value>";
					}
				}

				oss << "\n";

				if(std::empty(opt.get_description()) == false)
				{
					oss << "      " << opt.get_description() << "\n";
				}
				if(std::empty(opt.get_default_value()) == false)
				{
					oss << "      (default: " << opt.get_default_value() << ")\n";
				}

				if(opt.get_required() == true)
				{
					oss << "      (required)\n";
				}

				oss << "\n";
			}

			// add help option
			oss << "  --help, -h\n";
			oss << "      Display this help message\n";

			return oss.str();
		}

		auto add_option(const CommandLineOption& option) -> void
		{
			if(std::empty(option.get_name()) == true)
			{
				set_error_text("Option name cannot be empty");
				return;
			}
			if(options_.contains(option.get_name()) == true)
			{
				set_error_text("Option '" + option.get_name() + "' already exists");
				return;
			}

			ParsedOption parsed(option);

			// set default value if provided
			if(std::empty(option.get_default_value()) == false)
			{
				parsed.raw_values.emplace_back(option.get_default_value());
			}

			options_.emplace(option.get_name(), parsed);

			// register short name mapping
			if(std::empty(option.get_short_name()) == false)
			{
				if(short_to_long_.contains(option.get_short_name()) == true)
				{
					set_error_text("Short option '" + option.get_short_name() + "' already exists");
					return;
				}

				short_to_long_.emplace(option.get_short_name(), option.get_name());
			}
		}

	private:
		auto set_error_text(std::string error_message) -> void
		{
			if(error_text_ != error_message)
			{
				error_text_ = std::move(error_message);
			}
		}

		/// @brief Struct for
		struct ParsedOption
		{
			ParsedOption() = delete;
			explicit ParsedOption(CommandLineOption opt) : option(std::move(opt))
			{
			}

			// NOLINTBEGIN (misc-non-private-member-variables-in-classes)
			CommandLineOption option;
			std::vector<std::string> raw_values;
			bool was_set{false};
			// NOLINTEND
		};

		std::map<std::string, ParsedOption> options_;
		std::map<std::string, std::string> short_to_long_;
		std::string application_description_{"Command line parser"};
		std::string error_text_;
		// bool processed_{false};
	};
}