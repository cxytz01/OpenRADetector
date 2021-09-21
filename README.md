## OpenRA Lobbies Detector

This program is used to monitor the OpenRA room in real-time. Currently, only D2K is supported.

## Platforms
 * Windows (visual studio c++ 2019 with C++17)
 * Linux -- It should be supporting Linux, but it has not been verified.

## Dependence
* C++17 Complier support 
* [OpenSSL-Win64-1_1_1L library](https://slproweb.com/download/Win64OpenSSL-1_1_1L.exe)
* [spdlog library](https://github.com/gabime/spdlog)

## Install
0. Install spdlog with vcpkg  ---  https://github.com/gabime/spdlog#package-managers
    vcpkg install  spdlog:x64-windows
    
1. install openssl win64 1.1.1

2. download source code:    
    ```console
    $ git@github.com:cxytz01/OpenRADetector.git
    $ cd OpenRADetector
    ```
    Double click OpenRADetector.sln
    
3. Set the openssl include directory: Properties -> C/C++/General -> Additional Include Directories: D:\OpenSSL-Win64-1_1_1L\include

4. Set the openssl libray directory: Properties -> Linker/General -> Additional Library Directories: D:\OpenSSL-Win64-1_1_1L\lib\VC\static

## Usage samples

```console
OpenRADetector.exe F:\games\OpenRA\Dune2000.exe
```

0.When there are some waiting rooms, the console would be activated.

![example](.\assets\example.png)

1.Enter q|Q|quit|QUIT to exist the command line.

## Data Source

Grab from https://www.openra.net/games/ per 30s.

