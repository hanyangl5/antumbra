# Antumbra

(WIP)

Low level gpu compute framework API.

## Table of Contents

- [Antumbra](#antumbra)
  - [Table of Contents](#table-of-contents)
  - [Feature List](#feature-list)
  - [API Support](#api-support)
  - [Prerequisites](#prerequisites)
  - [Build From Source](#build-from-source)
  - [Contributing](#contributing)
  - [License](#license)

## Feature List

## API Support

<!-- Metal for macOS and iOS -->
Vulkan 1.3 on Windows<!-- , Linux, macOS -->
DirectX12 on Windows

## Prerequisites

- 64-bit operating system
- C++17 capable compiler
- CMake 3.21(or later)
- Vulkan SDK 1.3
- latest Windows SDK

## Build From Source

- Clone the project and its submodules.
`git clone https://github.com/hanyangl5/antumbra.git --recursive`
- Generate project using cmake
`cmake . -B build -G <generator>`

antumbra has been tested configured successfully with Ninja/Visual Studio under windows

- Compile and run tests or samples.

## Contributing

Feel free to dive in! [Open an issue](https://github.com/RichardLitt/standard-readme/issues/new) or submit PRs.

Antumbra follows the [Contributor Covenant](http://contributor-covenant.org/version/1/3/0/) Code of Conduct.

## License

[MIT](LICENSE) © hanyangl5
