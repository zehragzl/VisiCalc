# Contributing to VisiCalc

Thank you for considering contributing to VisiCalc! This document outlines the guidelines for contributing to this project.

## How to Contribute

### Reporting Bugs
1. Check if the issue already exists in the [Issues](https://github.com/zehragzl/VisiCalc/issues) tab
2. Create a new issue with a clear title and description
3. Include steps to reproduce, expected behavior, and actual behavior
4. Add your OS, compiler version, and terminal emulator info

### Suggesting Features
- Open an issue with the `enhancement` label
- Describe the feature and its use case
- Discuss the implementation approach if possible

### Submitting Changes
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/your-feature-name`)
3. Follow the coding style of the existing codebase
4. Commit with clear, descriptive messages
5. Push to your fork and submit a Pull Request

## Development Setup

```bash
# Clone the repository
git clone https://github.com/zehragzl/VisiCalc.git
cd VisiCalc

# Build in debug mode
make debug

# Run static analysis
make lint

# Format code
make format
```

## Coding Standards

- **C++ Standard**: C++17
- **Naming**: PascalCase for classes, camelCase for methods and variables
- **Comments**: Use `//` for single-line, `/* */` for multi-line explanations
- **Memory**: Prefer smart pointers (`std::unique_ptr`, `std::shared_ptr`) over raw pointers
- **Headers**: Use `#pragma once` or include guards

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
