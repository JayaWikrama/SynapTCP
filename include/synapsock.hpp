/*
 * $Id: synapsock.hpp,v 1.0.0 2024/09/12 20:51:47 Jaya Wikrama Exp $
 *
 * Copyright (c) 2024 Jaya Wikrama
 * jayawikrama89@gmail.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/**
 * @file
 * @brief Advance Socket Protocol (with framed data) library.
 *
 * This library provides a robust implementation of a socket communication protocol
 * that includes data framing. It is designed for reliable data exchange over socket
 * connections, supporting error detection and packet-based communication.
 *
 * @version 1.0.0
 * @date 2024-09-12
 * @author Jaya Wikrama
 */

#ifndef __SYNAPSOCK_HPP__
#define __SYNAPSOCK_HPP__

#include "socket.hpp"
#include "data-frame.hpp"

class SynapSock : public Socket {
  private:
    bool isFormatValid;
    DataFrame *frameFormat;
  public:
    /**
     * @brief Default constructor.
     *
     * This constructor initializes private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `address = 127.0.0.1` : Default address is set to localhost.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     */
    SynapSock();

    /**
     * @brief Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     */
    SynapSock(const unsigned char *address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     * @param[in] port The port of Socket interface.
     */
    SynapSock(const unsigned char *address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     */
    SynapSock(const std::vector <unsigned char> address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     * @param[in] port The port of Socket interface.
     */
    SynapSock(const std::vector <unsigned char> address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (in this case, a string in the form of a char pointer).
     */
    SynapSock(const char *address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (in this case, a string in the form of a char pointer).
     * @param[in] port The port of Socket interface.
     */
    SynapSock(const char *address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (string).
     */
    SynapSock(const std::string address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (string).
     * @param[in] port The port of Socket interface.
     */
    SynapSock(const std::string address, int port);

    /**
     * @brief Destructor.
     *
     * Releases any allocated memory.
     */
    ~SynapSock();

    /**
     * @brief Retrieves the memory address of the frame format.
     *
     * This function returns the address of the `frameFormat` data member.
     *
     * @return The memory address of the `frameFormat`.
     */
    DataFrame *getFormat();

    /**
     * @brief Release and destroy frame format pointer.
     *
     * This function will release memory previously allocated for `frameFormat`.
     *
     */
    void destroyFormat();

    /**
     * @brief Stops receiving framed socket data.
     *
     * This function sets up variables that act as indicators for the validity of the received socket data,
     * allowing it to stop receiving framed socket data from user space. It achieves this by using a post-execution
     * function configured within the `DataFrame` class.
     */
    void trigInvDataIndicator();

    /**
     * @brief Performs socket data receive operations with a custom frame format.
     *
     * This function executes socket data receiving operations using a specific frame format.
     * The receive socket data can be retrieved using the `__Serial::getBuffer__` method.
     *
     * @return 0 on success.
     * @return 1 if the port is not open.
     * @return 2 if a timeout occurs.
     * @return 3 if the frame format is not set up.
     * @return 4 if the frame data format is invalid.
     */
    int receiveFramedData();

    /**
     * @brief Performs socket data send operations with a custom frame format.
     *
     * This function executes socket data send operations using a specific frame format.
     *
     * @return 0 on success.
     * @return 1 if the port is not open.
     * @return 2 if a timeout occurs.
     * @return 3 if there is no data to send.
     */
    int sendFramedData();

    /**
     * @brief Retrieves a buffer of data receive and stored in the Framed Data within a specified range.
     *
     * This method extracts data from the buffer that has been successfully receive and stored in the Framed Data
     * within the specified range. This version is suitable for data with unique Frame Formats in each frame
     * (no duplicate Frame Types).
     *
     * @param begin Reference to the starting point for data extraction.
     * @param end Reference to the ending point for data extraction.
     * @return A vector containing the data.
     */
    std::vector <unsigned char> getSpecificBufferAsVector(DataFrame::FRAME_TYPE_t begin, DataFrame::FRAME_TYPE_t end);

    /**
     * @brief Overloaded method of __getSpecificBufferAsVector__.
     *
     * This method performs the same data extraction operation as the other overload but is designed to handle
     * cases where duplicate Frame Formats exist within the Framed Data. It retrieves data from the buffer
     * that has been successfully receive and stored within the specified range.
     *
     * @param begin Pointer to the starting point for data extraction.
     * @param end Pointer to the ending point for data extraction.
     * @return A vector containing the data.
     */
    std::vector <unsigned char> getSpecificBufferAsVector(const DataFrame *begin, const DataFrame *end);

    SynapSock& operator=(const DataFrame &obj);

    SynapSock& operator+=(const DataFrame &obj);

    SynapSock& operator+(const DataFrame &obj);

    DataFrame* operator[](int idx);

    DataFrame* operator[](DataFrame::FRAME_TYPE_t type);

    DataFrame* operator[](std::pair <DataFrame::FRAME_TYPE_t, int> params);
};

#endif
