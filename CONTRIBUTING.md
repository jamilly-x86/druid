# Contributing to Learn2D

Thank you for your interest in contributing to Learn2D! This document provides guidelines and information for contributors.

## Table of Contents

- [Getting Started](#getting-started)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Features](#suggesting-features)
- [Pull Requests](#pull-requests)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Documentation](#documentation)
- [Community Guidelines](#community-guidelines)

## Getting Started

Learn2D is an educational project focused on exploring modern C++ and graphics programming. We welcome contributions that align with these goals:

- Bug fixes and performance improvements
- Educational examples and tutorials
- Documentation improvements
- Cross-platform compatibility enhancements
- Modern C++ feature demonstrations

## Reporting Bugs

### Before Reporting

1. **Search existing issues**: Check if the bug has already been reported
2. **Use the latest version**: Ensure you're using the most recent commit
3. **Test on multiple platforms**: If possible, verify the issue on different operating systems

### How to Report

Create an issue on GitHub with the following information:

**Bug Report Template:**
```markdown
## Bug Description
Brief description of the issue

## Steps to Reproduce
1. Step one
2. Step two
3. Step three

## Expected Behavior
What should happen

## Actual Behavior
What actually happens

## Environment
- OS: [e.g., Windows 11, Ubuntu 22.04]
- Compiler: [e.g., MSVC 2022, GCC 12]
- CMake Version: [e.g., 3.27]
- Build Configuration: [e.g., debug-x64-windows-msvc]

## Additional Information
- Error messages or stack traces
- Screenshots (if applicable)
- Minimal code example to reproduce
```

## Suggesting Features

We welcome feature suggestions that enhance the educational value of Learn2D or demonstrate modern C++ techniques.

**Feature Request Template:**
```markdown
## Feature Description
Clear description of the proposed feature

## Educational Value
How does this feature help learn modern C++ or graphics programming?

## Use Case
Specific scenario where this feature would be useful

## Implementation Ideas
Any thoughts on how this could be implemented

## Alternatives Considered
Other approaches you've thought about
```

## Pull Requests

### Before Submitting

1. **Discuss major changes**: Open an issue first for significant modifications
2. **Follow coding standards**: Ensure your code matches project conventions
3. **Add tests**: Include unit tests for new functionality
4. **Update documentation**: Add or update relevant documentation
5. **Test cross-platform**: Verify changes work on Windows and Linux

### Pull Request Process

1. **Fork the repository** and create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes** following the coding standards

3. **Run the test suite**:
   ```bash
   cmake --build --preset your-preset
   ctest --preset your-preset
   ```

4. **Update documentation** as needed

5. **Submit the pull request** with a clear description:
   ```markdown
   ## Description
   Brief description of changes

   ## Type of Change
   - [ ] Bug fix
   - [ ] New feature
   - [ ] Breaking change
   - [ ] Documentation update

   ## Testing
   - [ ] Unit tests pass
   - [ ] Manual testing completed
   - [ ] Cross-platform testing (if applicable)

   ## Checklist
   - [ ] Code follows project style guidelines
   - [ ] Self-review completed
   - [ ] Documentation updated
   - [ ] Tests added/updated
   ```

## Development Setup

### Environment Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/learn2dengine/learn.git
   cd learn
   ```

2. **Install prerequisites** (see README.md for platform-specific instructions)

3. **Configure and build**:
   ```bash
   cmake --preset debug-x64-windows-msvc  # or appropriate preset
   cmake --build --preset debug-x64-windows-msvc
   ```

4. **Run tests**:
   ```bash
   ctest --preset debug-x64-windows-msvc
   ```

### Recommended Development Tools

- **IDE**: Visual Studio 2022, CLion, or VS Code with C++ extensions
- **Debugger**: Platform-appropriate debugger (Visual Studio Debugger, GDB, LLDB)
- **Static Analysis**: clang-tidy, PVS-Studio, or similar tools
- **Version Control**: Git with proper commit message conventions

## Coding Standards

### C++ Guidelines

- **C++20 Standard**: Use modern C++20 features where appropriate
- **Modules**: Prefer C++20 modules over traditional headers for new code
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `GraphicsWindow`)
  - Functions/Variables: `camelCase` (e.g., `createWindow()`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_BUFFER_SIZE`)
  - Namespaces: `lowercase` (e.g., `druid::graphics`)

### Code Style

- **Indentation**: Use tabs (4 spaces equivalent)
- **Line Length**: Maximum 120 characters
- **Braces**: Allman style (opening brace on new line)
- **Comments**: Use `//` for single-line, `/* */` for multi-line
- **Documentation**: Use Doxygen-style comments for public APIs

### Example Code Style

```cpp
namespace druid::graphics
{
    /// @brief Represents a graphics window
    class Window
    {
    public:
        /// @brief Creates a new window
        /// @param width Window width in pixels
        /// @param height Window height in pixels
        /// @return True if successful, false otherwise
        bool create(int width, int height);

    private:
        int m_width{0};
        int m_height{0};
    };
}
```

## Testing

### Unit Testing

- **Framework**: Google Test (GTest)
- **Location**: Tests should be placed in `test/` subdirectories alongside source files
- **Naming**: Test files should end with `_test.cpp`
- **Coverage**: Aim for good test coverage of public APIs

### Test Structure

```cpp
#include <gtest/gtest.h>
#include "YourModule.h"

class YourModuleTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Setup code
    }
};

TEST_F(YourModuleTest, TestBasicFunctionality)
{
    // Arrange
    auto instance = YourModule{};
    
    // Act
    auto result = instance.doSomething();
    
    // Assert
    EXPECT_TRUE(result);
}
```

### Running Tests

```bash
# Build with tests enabled
cmake --preset debug-x64-windows-msvc -DBUILD_TESTING=ON

# Run all tests
ctest --preset debug-x64-windows-msvc

# Run specific test
ctest --preset debug-x64-windows-msvc -R YourModuleTest
```

## Documentation

### Types of Documentation

1. **API Documentation**: Doxygen comments in source code
2. **Tutorials**: Step-by-step guides in `docs/tutorials/`
3. **Examples**: Working code examples in `examples/`
4. **README Updates**: Keep README.md current with new features

### Documentation Guidelines

- **Clear and Concise**: Write for beginners in graphics programming
- **Code Examples**: Include working code samples
- **Cross-References**: Link related concepts and APIs
- **Platform Notes**: Mention platform-specific considerations

### Building Documentation

```bash
# Generate API documentation (if Doxygen is configured)
cmake --build --preset your-preset --target docs

# Preview documentation locally
# Documentation will be generated in build/docs/
```

## Community Guidelines

### Code of Conduct

- **Respectful**: Treat all contributors with respect and professionalism
- **Constructive**: Provide helpful feedback and suggestions
- **Educational**: Remember this is a learning-focused project
- **Inclusive**: Welcome contributors of all skill levels

### Communication Channels

- **Issues**: GitHub Issues for bug reports and feature requests
- **Discussions**: GitHub Discussions for questions and general discussion
- **Pull Requests**: Code review and collaboration
- **Documentation**: In-code comments and README updates

### Getting Help

If you need help contributing:

1. **Check existing documentation** in README.md and source comments
2. **Search existing issues** for similar questions
3. **Create a discussion** on GitHub for general questions
4. **Open an issue** for specific bugs or feature requests

## Recognition

Contributors will be recognized in:
- GitHub contributor graphs
- Release notes for significant contributions
- Project documentation acknowledgments

Thank you for helping make Learn2D a better educational resource for the C++ and graphics programming community!
