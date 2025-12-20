# Contributing to RequestFlow

Thank you for your interest in contributing to RequestFlow! This document provides guidelines and instructions for contributing to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Coding Standards](#coding-standards)
- [Pull Request Process](#pull-request-process)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Enhancements](#suggesting-enhancements)

## Code of Conduct

- Be respectful and inclusive
- Welcome newcomers and help them get started
- Focus on constructive feedback
- Respect differing viewpoints and experiences

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/YOUR-USERNAME/RequestFlow.git
   cd RequestFlow
   ```
3. **Add upstream remote**:
   ```bash
   git remote add upstream https://github.com/fatehmtd/RequestFlow.git
   ```

## Development Setup

### Prerequisites

- Qt 5.15.2+ or Qt 6.x
- C++17 compatible compiler (MSVC 2019+, GCC, or Clang)
- Git

### Building the Project

#### Windows (Visual Studio)
```batch
# Open RequestFlow.sln in Visual Studio
# Or use command line:
build.bat
```

#### Linux/macOS
```bash
cd RequestFlowApp
qmake RequestFlow.pro
make
```

### Project Structure

- `CoreModel/` - Core data models, execution engine, and business logic
- `CoreView/` - UI components, node widgets, and visual elements
- `ExecutionEngine/` - HTTP request execution and workflow processing
- `RequestFlow/` - Main application, windows, and application logic

## How to Contribute

### Types of Contributions

We welcome various types of contributions:

- **Bug Fixes**: Fix issues reported in the issue tracker
- **New Features**: Implement new node types, functionality, or tools
- **Documentation**: Improve README, code comments, or create tutorials
- **UI/UX**: Enhance the user interface and user experience
- **Testing**: Add unit tests or integration tests
- **Performance**: Optimize code for better performance
- **Platform Support**: Improve cross-platform compatibility

### Contribution Workflow

1. **Check existing issues** or create a new one to discuss your contribution
2. **Create a feature branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes** with clear, focused commits
4. **Test your changes** thoroughly
5. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```
6. **Submit a Pull Request** from your fork to the main repository

## Coding Standards

### C++ Guidelines

- Use **C++17** features where appropriate
- Follow the existing code style and formatting
- Use meaningful variable and function names
- Add comments for complex logic
- Prefer `const` correctness
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) for memory management

### Qt Specific

- Follow Qt naming conventions (camelCase for methods, _underscore prefix for private members)
- Use Qt's signal/slot mechanism for event handling
- Properly manage Qt object ownership and parent-child relationships
- Use `Q_OBJECT` macro for classes that need signals/slots

### Code Organization

- Keep classes focused on a single responsibility
- Separate UI logic from business logic
- Use header files (.h) for declarations and source files (.cpp) for implementations
- Include guards or `#pragma once` in headers

### Example Code Style

```cpp
class CustomNode : public Node
{
    Q_OBJECT
public:
    CustomNode(Graph* parent);
    ~CustomNode();
    
    void setValue(const QString& value);
    QString getValue() const;

signals:
    void valueChanged(const QString& value);

private:
    void updateInternalState();
    
    QString _value;
    int _counter;
};
```

## Pull Request Process

1. **Update documentation** if you're changing functionality
2. **Ensure the code builds** on your platform without warnings
3. **Test your changes** - manually test the affected features
4. **Write a clear PR description**:
   - What does this PR do?
   - Why is this change needed?
   - How has it been tested?
   - Screenshots (if UI changes)
5. **Link related issues** using keywords like "Fixes #123"
6. **Be responsive** to review feedback

### PR Title Format

Use clear, descriptive titles:
- `Fix: Crash when deleting connected nodes`
- `Feature: Add REST authentication node`
- `Docs: Update build instructions for macOS`
- `Refactor: Simplify message passing logic`

## Reporting Bugs

When reporting bugs, please include:

1. **Description**: Clear description of the bug
2. **Steps to Reproduce**: Detailed steps to recreate the issue
3. **Expected Behavior**: What should happen
4. **Actual Behavior**: What actually happens
5. **Environment**:
   - OS and version
   - Qt version
   - RequestFlow version
   - Compiler used
6. **Screenshots/Logs**: If applicable
7. **Possible Fix**: If you have ideas (optional)

## Suggesting Enhancements

We welcome feature suggestions! Please provide:

1. **Use Case**: Why is this feature needed?
2. **Proposed Solution**: How should it work?
3. **Alternatives**: Other approaches you've considered
4. **Additional Context**: Screenshots, mockups, or examples

## Questions?

If you have questions about contributing:
- Open a GitHub issue with the "question" label
- Contact the maintainer: fateh@requestflow.dev

## Recognition

Contributors will be recognized in:
- GitHub contributors list
- Release notes
- Project documentation (for significant contributions)

Thank you for helping make RequestFlow better!
