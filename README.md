# RequestFlow

**REST API Testing for Human Beings**

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey)
![Qt](https://img.shields.io/badge/Qt-6.x%20%7C%205.15.2+-green.svg)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

[![Build (Linux)](https://github.com/fatehmtd/RequestFlow/actions/workflows/build-linux.yml/badge.svg?branch=main)](https://github.com/fatehmtd/RequestFlow/actions/workflows/build-linux.yml)
[![Build (macOS)](https://github.com/fatehmtd/RequestFlow/actions/workflows/build-macos.yml/badge.svg?branch=main)](https://github.com/fatehmtd/RequestFlow/actions/workflows/build-macos.yml)
[![Build (Windows)](https://github.com/fatehmtd/RequestFlow/actions/workflows/build-windows.yml/badge.svg?branch=main)](https://github.com/fatehmtd/RequestFlow/actions/workflows/build-windows.yml)

RequestFlow is a visual, node-based REST API testing and workflow automation tool. Instead of writing scripts or managing complex configuration files, you design API test workflows by connecting nodes on a canvas. Think of it as a flowchart editor for your HTTP requests.

<p align="center">
  <img src="web/images/main-ui.png" alt="RequestFlow Main Interface" width="800">
  <br>
  <em>RequestFlow's visual node-based workflow editor</em>
</p>

---

## Why RequestFlow?

Traditional API testing tools force you to think in terms of collections, folders, and request lists. RequestFlow lets you think visually. Build complex workflows by dragging nodes onto a canvas and connecting them together. See your data flow in real-time, add conditional logic, run custom scripts, and validate responses—all without leaving the visual editor.

Perfect for:
- Testing complex API workflows with multiple interdependent requests
- Automating integration tests with visual feedback
- Debugging API flows by inspecting data at each step
- Building reusable test scenarios that are easy to understand and maintain

---

## Features

### Visual Workflow Design
Build API test flows using a drag-and-drop node editor. Connect HTTP requests, data transformations, conditionals, and assertions visually. No more wrestling with nested JSON configurations or scattered test scripts.

<p align="center">
  <img src="web/images/example-flow.png" alt="Example Workflow" width="700">
  <br>
  <em>Complex API workflows made simple with visual connections</em>
</p>

### Node Types

- **Endpoint Nodes** - Full-featured HTTP client supporting all REST methods (GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS)
  - Multiple authentication methods: Basic Auth, Bearer tokens, OAuth
  - Custom headers, timeouts, user agents, and content types
  - Response validation with configurable accepted/rejected status codes
  - Built-in request/response logging

- **Payload Nodes** - Flexible data source management
  - Load messages from files or define inline JSON
  - Inject structured data into your workflow at any point
  - Perfect for test data sets and mock responses

- **Script Nodes** - JavaScript execution engine for data transformation
  - Full JavaScript support with access to message context
  - Transform API responses, extract values, manipulate data structures
  - No external dependencies required

- **Assertion Nodes** - JavaScript-based testing framework
  - Write custom assertions to validate API behavior
  - Access response data, headers, and status codes
  - Build comprehensive test suites visually

- **Conditional Nodes** - Smart workflow branching
  - JavaScript-powered conditions for dynamic execution paths
  - Enable/disable branches based on runtime data
  - Handle different scenarios in a single workflow

- **Delay Nodes** - Precise timing control
  - Add configurable delays between operations
  - Essential for rate-limited APIs and sequential testing
  - Millisecond-level precision

- **Viewer Nodes** - Advanced data inspection and export
  - Real-time message viewing at any workflow step
  - JSONPath filtering for complex data structures
  - Export capabilities to save responses and debug data

- **External Nodes** - Workflow composition and reusability
  - Reference nodes from other graphs to eliminate duplication
  - Configurable execution conditions (always run, cache-based, or custom logic)
  - Build libraries of reusable test components

- **Group Nodes** - Visual organization
  - Color-coded grouping for related nodes
  - Keep complex workflows clean and maintainable
  - Collapse/expand groups for better overview

<p align="center">
  <img src="web/images/config-endpoint.png" alt="Endpoint Configuration" width="600">
  <br>
  <em>Configuring HTTP requests with authentication and headers</em>
</p>

### Environment Management
Switch between development, staging, and production environments with a single click. Define environment-specific variables and use them anywhere with `{variable}` syntax. No more hardcoded URLs or credentials.

<p align="center">
  <img src="web/images/prod-env.png" alt="Environment Management" width="600">
  <br>
  <em>Manage multiple environments with ease</em>
</p>

### Message Flow Tracking
Every piece of data flowing through your workflow is tracked. See request bodies, response headers, query parameters, path variables, and custom context data at each step.

<p align="center">
  <img src="web/images/data-viz.png" alt="Data Visualization" width="600">
  <br>
  <em>Inspect response data at any workflow step</em>
</p>

### JavaScript Integration
Built-in JavaScript engine with custom libraries for path manipulation and assertions. Transform data, validate responses, and implement custom logic without external dependencies.

<p align="center">
  <img src="web/images/scripting.png" alt="JavaScript Scripting" width="600">
  <br>
  <em>Write custom JavaScript for data transformation and logic</em>
</p>

### Advanced Data Filtering
Filter and query complex JSON responses using JSONPath expressions. Export filtered data for further analysis or testing.

<p align="center">
  <img src="web/images/view-filtering.png" alt="JSONPath Filtering" width="600">
  <br>
  <em>Filter data with powerful JSONPath expressions</em>
</p>

### Project Management
Save your workflows as `.rqfl` project files. Organize multiple test scenarios, version control your tests, and share them with your team.

### Dark/Light Themes
Work comfortably in your preferred theme. The interface adapts to your choice with full syntax highlighting support.

---

## Getting Started

### Download

Pre-built binaries are available for Windows, macOS, and Linux:

**[Download from GitHub Releases](https://github.com/fatehmtd/RequestFlow/releases)**

- **Windows:** Installer with all dependencies included (Windows 10/11, 64-bit)
- **macOS:** Universal binary for Intel and Apple Silicon (macOS 10.14+)
- **Linux:** AppImage for major distributions (Ubuntu 20.04+, Fedora, etc.)

### Installation Instructions

#### macOS
1. Download `RequestFlow-macOS.zip` from the releases page
2. Extract the ZIP file (double-click or use `unzip RequestFlow-macOS.zip`)
3. Drag `RequestFlow.app` to your Applications folder (optional)
4. **Important:** On first launch, right-click the app and select "Open" (this is required for unsigned apps)
5. Click "Open" in the security dialog that appears
6. The app will now run normally - you only need to do this once

**Note:** Since the app is currently unsigned, macOS Gatekeeper will prevent it from opening if you double-click it directly. Using right-click → Open bypasses this restriction.

#### Linux
1. Download `RequestFlow-Linux-x86_64.AppImage` from the releases page
2. Make it executable: `chmod +x RequestFlow-Linux-x86_64.AppImage`
3. Run it: `./RequestFlow-Linux-x86_64.AppImage`

#### Windows
1. Download `RequestFlow-Windows-x64.zip` from the releases page
2. Extract the ZIP file to your preferred location
3. Run `RequestFlow.exe`

### Building from Source

Requirements:
- Qt 5.15.2+ or Qt 6.x
- C++17 compatible compiler (MSVC 2019+, GCC, or Clang)
- Qt modules: Core, GUI, Network, Widgets, QML, SVG, OpenGL

```bash
git clone https://github.com/fatehmtd/RequestFlow.git
cd RequestFlow/RequestFlowApp
qmake RequestFlow.pro
make
```

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed build instructions and development setup.

---

## Quick Example

1. Create a new project
2. Drag an **Endpoint Node** onto the canvas
3. Configure it with your API URL (use `{baseUrl}/users` with environment variables)
4. Add a **Viewer Node** to inspect the response
5. Connect the nodes
6. Hit **Run** and watch the data flow

Want to test an authenticated endpoint? Add another Endpoint Node, connect it to the first, and configure Bearer token authentication. Need to validate the response? Drop in an Assertion Node and write a simple JavaScript check.

---

## Project Status

⚠️ **Early Release Notice**

RequestFlow is in active development and represents an early release. While the core features are functional and ready for testing, you may encounter bugs, rough edges, or undefined behavior. This is expected for a young project, and we're working continuously to improve stability and add polish.

**What this means for you:**
- The application is usable for real API testing workflows
- Some edge cases may not be handled gracefully yet
- Features are being added and refined regularly
- Your bug reports and feedback directly improve the project
- Breaking changes may occur as we refine the architecture

**We need your help!** This is a community-driven project, and contributions are essential:
- **Report bugs** you encounter via [GitHub Issues](https://github.com/fatehmtd/RequestFlow/issues)
- **Share feature ideas** and use cases to shape the roadmap
- **Contribute code** for bug fixes, features, or improvements
- **Improve documentation** by clarifying confusing parts or adding examples
- **Test on your platform** and report platform-specific issues

By using RequestFlow in this early stage, you're helping build a better tool for the entire community. We appreciate your patience and contributions!

---

## Contributing

We welcome contributions! Whether it's bug fixes, new features, documentation, or UI improvements, your help makes RequestFlow better for everyone.

Ways to contribute:
- Report bugs and suggest features via [GitHub Issues](https://github.com/fatehmtd/RequestFlow/issues)
- Implement new node types or workflow capabilities
- Improve the UI/UX
- Write documentation and tutorials
- Fix platform-specific issues
- Add test coverage

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on code style, pull requests, and development workflow.

---

## Technology Stack

Built with modern C++ and the Qt framework for cross-platform compatibility:

- **Qt 6.x** (compatible with Qt 5.15.2+) for UI and networking
- **C++17** for core logic
- **QGraphicsView** for the node-based visual editor
- **QJSEngine** for JavaScript execution
- **QNetworkAccessManager** for HTTP client functionality

---

## License

RequestFlow is open source software licensed under the [MIT License](LICENSE).

---

## Author

Created and maintained by **Fateh Benmerzoug, Ph.D**

- Email: fatehmtd+requestflow@gmail.com
- Website: [fatehmtd.github.io/RequestFlow](https://fatehmtd.github.io/RequestFlow)
- GitHub: [@fatehmtd](https://github.com/fatehmtd)

---

## Acknowledgments

Built with the Qt Framework. Special thanks to the open source community for inspiration and support.

---

**Start building visual API workflows today. Download RequestFlow and see the difference.**
