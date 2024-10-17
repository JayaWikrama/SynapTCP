# SynapTCP Library

SynapTCP is a library for socket (TCP/IP) communication that designed to support and simplify software development in Linux environments. This library provides a straightforward interface and configuration, making it easy to integrate into various applications. Additionally, SynapTCP supports advanced socket (TCP/IP) communication, including communication using specific protocols (framed data).

The key functionalities include:

- Initialization and configuration of TCP/IP Server.
- Initialization and configuration of TCP/IP Client.
- Sending and receiving data over TCP/IP connections.
- Error handling and diagnostics for TCP/IP communication on client and server side.
- Utility functions for managing TCP/IP buffers and flow control.
- Provides a robust implementation of a socket communication protocol that includes data framing.
- Provides simple method to SSL Implementation.

## Installation

Follow these steps to install the necessary packages and dependencies.

### Installing Compiler and CMake

1. Install the Compiler:

```bash
sudo apt install build-essential g++ binutils
```

3. Install CMake:

```bash
sudo apt install make cmake
```

### Installing Google Test

1. Clone repository:

```bash
git clone https://github.com/google/googletest.git -b v1.14.0
```

2. Navigate to the cloned repository directory:

```bash
cd googletest
```

3. Build the Google Test Library:

```bash
mkdir build
cd build
cmake ..
make
```

4. Install Google Test Library:

```bash
sudo make install
```

5. Return to the root directory:

```bash
cd ../../
```

## Building the Library for Test

1. Clone the main repository:

```bash
git clone https://github.com/JayaWikrama/SynapTCP.git
```

2. Navigate to the project directory:

```bash
cd SynapTCP
```

3. Initialize and update all Git submodules:

```bash
git submodule init 
git submodule update --init --recursive
```

4. If you want to use the latest source code (same as the remote repository), run:

```bash
git submodule update --remote
```

5. Create a build directory:

```bash
mkdir build
cd build
```

6. Configure CMake:

```bash
cmake ..
```

7. Build the library:

```bash
make
```

8. Run the unit tests to ensure all functions are working correctly:

```bash
./SynapTCP-test
```

Sample test output:

```bash
[==========] Running 80 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 39 tests from TCPSimpleTest
[ RUN      ] TCPSimpleTest.DefaultConstructor_1
Create Echo Server Thread
Echo Server Thread Stoped
[       OK ] TCPSimpleTest.DefaultConstructor_1 (125 ms)
[ RUN      ] TCPSimpleTest.CustomConstructor_1
Create Echo Server Thread
Echo Server Thread Stoped
[       OK ] TCPSimpleTest.CustomConstructor_1 (125 ms)
[ RUN      ] TCPSimpleTest.CustomConstructor_2
Create Echo Server Thread
Echo Server Thread Stoped
[       OK ] TCPSimpleTest.CustomConstructor_2 (125 ms)
[ RUN      ] TCPSimpleTest.CustomConstructor_3
Create Echo Server Thread
Echo Server Thread Stoped
[       OK ] TCPSimpleTest.CustomConstructor_3 (125 ms)
[ RUN      ] TCPSimpleTest.CustomConstructor_4
Create Echo Server Thread
Echo Server Thread Stoped
[       OK ] TCPSimpleTest.CustomConstructor_4 (125 ms)
[ RUN      ] TCPSimpleTest.CustomConstructor_5
Create Echo Server Thread
Echo Server Thread Stoped
[       OK ] TCPSimpleTest.CustomConstructor_5 (125 ms)
....
....
....
[ RUN      ] SSLSimpleTest.negativeCommunicationTest_no_input_bytes_available
Create Echo Server Thread
New client accepted
A client disconnected from server
Echo Server Thread Stoped
[       OK ] SSLSimpleTest.negativeCommunicationTest_no_input_bytes_available (381 ms)
[ RUN      ] SSLSimpleTest.negativeCommunicationTest_rcvStopBytes
Create Echo Server Thread
New client accepted
Data Received form 127.0.0.1:42452
Echo Server Thread Stoped
[       OK ] SSLSimpleTest.negativeCommunicationTest_rcvStopBytes (175 ms)
[----------] 22 tests from SSLSimpleTest (5823 ms total)

[----------] Global test environment tear-down
[==========] 80 tests from 3 test suites ran. (16438 ms total)
[  PASSED  ] 80 tests.
```

## Using the Library

One way to use this library is by integrating it into your main application as a Git submodule. Here’s an example of how to create a new project TCP/IP Server and integrate the SynapTCP library into it:

1. Create the project tree.

```bash
mkdir AdvanceTCPServer
cd AdvanceTCPServer
mkdir src
mkdir include
mkdir external
git init .
```

2. Add the SynapTCP library as an external library.

```bash
git submodule add https://github.com/JayaWikrama/SynapTCP.git external/SynapTCP
git submodule update --init --recursive
```

3. Create the main application source code.

```bash
nano src/main.cpp
```

Here is the sample source code.

```cpp
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "tcp-server.hpp"

const std::string errorMessage[] = {
    "Socket Port Has Not Been Initialized",
    "Timeout",
    "Frame Format Has Not Been Setup"
};

extern void displayData(const std::vector <unsigned char> &data);
extern void socketSetupDataFrameProtocol(SynapSock &obj);

void receptionCallbackFunction(SynapSock &connection, void *param){
    static int ret = 0;
    std::vector <unsigned char> data;
    std::cout << "Receiving data form " << connection.getAddress() << ":" << connection.getPort() << std::endl;
    if (connection.getFormat() == nullptr) socketSetupDataFrameProtocol(connection);
    ret = connection.receiveFramedData();
    if (ret == 0){
        std::cout << "Success to receive data: ";
        displayData(connection.getBufferAsVector());
        connection.sendData(connection.getBufferAsVector());
    }
    else {
        if (ret == 4){
            std::cout << "Failed to receive data (invalid data). Details:" << std::endl;
            if (connection.getBuffer(data) > 0){
                std::cout << "    Received Data: ";
                displayData(data);
            }
            if (connection.getRemainingBuffer(data) > 0){
                std::cout << "    Remaining Received Data: ";
                displayData(data);
            }
            std::cout << std::endl;
        }
        else {
            std::cout << errorMessage[ret - 1] << std::endl;
        }
    }
    connection.destroyFormat();
}

int main(int argc, char **argv){
    if (argc != 2){
        std::cout << "cmd: " << argv[0] << " <port>" << std::endl;
        exit(0);
    }
    TCPServer server("0.0.0.0", atoi(argv[1]));
    server.setReceptionHandler(&receptionCallbackFunction, nullptr, true);
    TCPServer::SERVER_EVENT_t event;
    SynapSock *activeConnection = nullptr;
    if (server.init() != 0){
        std::cerr << "Failed to initialize server" << std::endl;
        exit(1);
    }
    do {
        event = server.eventCheck(125);
        if (event == TCPServer::EVENT_CONNECT_REQUEST){
            std::cout << "New client accepted" << std::endl;
        }
        else if (event == TCPServer::EVENT_CLIENT_DISCONNECTED){
            std::cout << "A client disconnected from server" << std::endl;
        }
    } while (1);
    return 0;
}
```

Save file by press `[Ctrl + o]` on your keyboard and then press `[Enter]`. After that press `[Ctrl + x]`.

4. Create the data formating source code.

```bash
nano src/data-formating.cpp
```

Here is the sample source code.

```cpp
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "tcp-client.hpp"
#include "tcp-server.hpp"
#include "synapsock.hpp"

void displayData(const std::vector <unsigned char> &data){
    for (auto i = data.begin(); i < data.end(); ++i){
        std::cout << std::hex << (int) *i;
        std::cout << " ";
    }
    std::cout << std::endl;
}

void crc16(DataFrame &frame, void *ptr) {
    /* Get SynapSock object form function param */
    SynapSock *tmp = (SynapSock *) ptr;
    /* Initialize crc16 param */
    unsigned short crc = 0x0000;
    unsigned short poly = 0x1021;
    /* Get data from Start Bytes until Data */
    std::vector <unsigned char> data = tmp->getSpecificBufferAsVector(DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA);
    std::cout << "Data from which the CRC value will be calculated:" << std::endl;
    displayData(data);
    /* Calculate crc16 */
    for (const auto &byte : data) {
        crc ^= (static_cast<unsigned short>(byte) << 8);
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    /* Compare received CRC with calculated CRC */
    unsigned short rcvCRC = 0;
    frame.getData((unsigned char *) &rcvCRC, 2);
    if (rcvCRC != crc){
        /* invalid crc -> trigger stop flag to __readFramedData__ method */
        tmp->trigInvDataIndicator();
        std::cout << "CRC16 Invalid (0x" << std::hex << rcvCRC << " != 0x" << std::hex << crc << ")" << std::endl;
    }
}

void crc16ForClientSide(DataFrame &frame, void *ptr){

}

std::vector <unsigned char> base16decode(const char *data){
    std::vector <unsigned char> result;
    if (strlen(data) % 2 == 1) return result;
    unsigned char tmp = 0x00;
    while (*data != 0x00){
        if (*data >= '0' && *data <= '9') tmp = (*data) - '0';
        else if (*data >= 'a' && *data <= 'z') tmp = (*data) - 'a';
        else if (*data >= 'A' && *data <= 'Z') tmp = (*data) - 'A';
        data++;
        tmp *= 16;
        if (*data >= '0' && *data <= '9') tmp += ((*data) - '0');
        else if (*data >= 'a' && *data <= 'z') tmp += ((*data) - 'a') + 10;
        else if (*data >= 'A' && *data <= 'Z') tmp += ((*data) - 'A') + 10;
        data++;
        result.push_back(tmp);
    }
    return result;
}

void setupLengthByCommand(DataFrame &frame, void *ptr){
    int data = 0;
    /* Get SynapSock object form function param */
    SynapSock *tmp = (SynapSock *) ptr;
    /* Get DataFrame target */
    DataFrame *target = (*tmp)[DataFrame::FRAME_TYPE_DATA];
    if (target == nullptr) return;
    frame.getData((unsigned char *) &data, 1);
    if (data == 0x35){
        /* setup 3 as data size of DataFrame::FRAME_TYPE_DATA */
        target->setSize(3);
    }
    else if (data == 0x36){
        /* setup 3 as data size of DataFrame::FRAME_TYPE_DATA */
        target->setSize(2);
    }
    else {
        /* invalid value found -> trigger stop flag to __readFramedData__ method */
        tmp->trigInvDataIndicator();
    }
}

void socketSetupDataFrameProtocol(SynapSock &obj){
    /* Frame Data Format / Protocol Example
     * | Start  Bytes |   Command   |  Main Data   | CRC16 Validator | Stop Bytes |
     * |:-------------|:------------|:-------------|:----------------|:-----------|
     * |    4 bytes   |   1 byte    |   N bytes    |     2 bytes     | 4 bytes    |
     * |  0x31323334  | 0x35 / 0x36 | based on Cmd |  init = 0x0000  | 0x39302D3D |
     *
     * If Command = 0x35, then the Main Data length is 3 bytes.
     * If Command = 0x36, then the Main Data length is 2 bytes.
     * If Command is not equal to 0x35 and 0x36, then the data is invalid.
     *
     * example: 3132333435363738159039302D3D
     */
    /* Configure Frame Format */
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, 1);
    /* Setup the handler function to determine the data length of DataFrame::FRAME_TYPE_DATA.
     * This function is called after all data from DataFrame::FRAME_TYPE_COMMAND is received.
     */
    cmdBytes.setPostExecuteFunction((const void *) &setupLengthByCommand, (void *) &obj);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame crcValidatorBytes(DataFrame::FRAME_TYPE_VALIDATOR, 2);
    /* Setup the handler function to validate Data by using crc16 validation.
     * This function is called after all data from DataFrame::FRAME_TYPE_VALIDATOR is received.
     */
    crcValidatorBytes.setPostExecuteFunction((const void *) &crc16, (void *) &obj);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    /* Setup Frame Format to SynapSock com */
    obj = startBytes + cmdBytes + dataBytes + crcValidatorBytes + stopBytes;
}
```

Save file by press `[Ctrl + o]` on your keyboard and then press `[Enter]`. After that press `[Ctrl + x]`.

5. Create a `CMakeLists.txt` file.

```bash
nano CMakeLists.txt
```

Here is `cmake` configuration:

```bash
cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "1")
set(PROJECT_VERSION_PATCH "0")
project(AdvanceTCPServer VERSION 1.1.0 LANGUAGES CXX)

include(FindPkgConfig)

# Set the default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Find and Check Library
find_package(PkgConfig REQUIRED)

# Specify the source files
set(SOURCE_FILES
    src/main.cpp
    src/data-formating.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/SynapTCP/include>
  $<INSTALL_INTERFACE:include>
)

# Ensure SynapTCP is built before the executable
add_dependencies(${PROJECT_NAME} SynapTCP-lib)

# Link the executable with the library
target_link_libraries(${PROJECT_NAME} PRIVATE SynapTCP-lib DataFrame-lib)
target_link_libraries(${PROJECT_NAME} PUBLIC -lpthread -lssl)

add_subdirectory(external/SynapTCP EXCLUDE_FROM_ALL)
set(USE_EXE_FUNC ON)
set(USE_POST_EXE_FUNC ON)
add_definitions(-D__USE_EXE_FUNC)
add_definitions(-D__USE_POST_FUNC)
add_definitions(-D__STCP_SSL__)

# Set compiler and linker flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -O3")
set(CMAKE_USE_RELATIVE_PATHS OFF)
```

Save file by press `[Ctrl + o]` on your keyboard and then press `[Enter]`. After that press `[Ctrl + x]`.

### Explanation

- Line 29: The `$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/SynapTCP/include>` command adds a reference to the header file location of the SynapTCP Library. This ensures that during the build process, CMake can locate the SynapTCP headers correctly.

- Line 34: The `add_dependencies(${PROJECT_NAME} SynapTCP-lib)` command specifies that the SynapTCP Library must be built before the main application. This ensures that the SynapTCP Library is compiled before the main project depends on it.

- Line 37: The `target_link_libraries(${PROJECT_NAME} PRIVATE SynapTCP-lib DataFrame-lib)` command links the SynapTCP Library and the DataFrame Library (a dependency of the SynapTCP Library) to the main application as private libraries. This means that these libraries are used internally by the main project.

- Line 40: The `add_subdirectory(external/SynapTCP EXCLUDE_FROM_ALL)` command adds the external/SynapTCP directory as part of the main project. The EXCLUDE_FROM_ALL option prevents CMake from building this subdirectory by default unless explicitly requested.

5. Build the project.

```bash
mkdir build
cd build
cmake ..
make
```

6. Run the application.

```bash
./AdvanceTCPServer <port>
```

Example:

```bash
./AdvanceTCPServer 8080
```
