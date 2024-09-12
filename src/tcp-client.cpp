/*
 * $Id: tcp-client.cpp,v 1.0.1 2024/09/11 12:51:47 Jaya Wikrama Exp $
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

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "tcp-client.hpp"

/**
 * @brief Default constructor.
 *
 * This constructor initializes private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `port = 3000` : Port is set to 3000 as its default value.
 * - `address = 127.0.0.1` : Default address is set to localhost.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 */
TCPClient::TCPClient(){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Custom constructor.
 *
 * This constructor initializes some private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `port = 3000` : Port is set to 3000 as its default value.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
 */
TCPClient::TCPClient(const unsigned char *address) : SynapSock(address){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Overloading of Custom constructor.
 *
 * This constructor initializes some private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
 * @param[in] port The port of TCPClient/IP interface.
 */
TCPClient::TCPClient(const unsigned char *address, int port) : SynapSock(address, port){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Overloading of Custom constructor.
 *
 * This constructor initializes some private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `port = 3000` : Port is set to 3000 as its default value.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
 */
TCPClient::TCPClient(const std::vector <unsigned char> address) : SynapSock(address){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Overloading of Custom constructor.
 *
 * This constructor initializes some private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
 * @param[in] port The port of TCPClient/IP interface.
 */
TCPClient::TCPClient(const std::vector <unsigned char> address, int port) : SynapSock(address, port){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Overloading of Custom constructor.
 *
 * This constructor initializes some private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `port = 3000` : Port is set to 3000 as its default value.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 * @param[in] address The address in the form of an IP address or domain (in this case, a string in the form of a char pointer).
 */
TCPClient::TCPClient(const char *address) : SynapSock(address){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Overloading of Custom constructor.
 *
 * This constructor initializes some private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 * @param[in] address The address in the form of an IP address or domain (in this case, a string in the form of a char pointer).
 * @param[in] port The port of TCPClient/IP interface.
 */
TCPClient::TCPClient(const char *address, int port) : SynapSock(address, port){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Overloading of Custom constructor.
 *
 * This constructor initializes some private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `port = 3000` : Port is set to 3000 as its default value.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 * @param[in] address The address in the form of an IP address or domain (string).
 */
TCPClient::TCPClient(const std::string address) : SynapSock(address){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Overloading of Custom constructor.
 *
 * This constructor initializes some private and protected data members and parameters to their default values, including:
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 * @param[in] address The address in the form of an IP address or domain (string).
 * @param[in] port The port of TCPClient/IP interface.
 */
TCPClient::TCPClient(const std::string address, int port) : SynapSock(address, port){
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
}

/**
 * @brief Destructor.
 *
 * This destructor is responsible for releasing any memory that has been allocated during the object's lifetime.
 * It ensures that all allocated resources are properly freed, preventing memory leaks.
 */
TCPClient::~TCPClient(){
  // do nothing
}

/**
 * @brief Connect to TCP/IP Server with/without timeout.
 *
 * This method is called automatically when the `clientInit` method is called.
 *
 * @return `true` if success
 * @return `false` if fail or timeout
 */
bool TCPClient::connectToServer(){
  if (this->tvTimeout.tv_sec > 0 || this->tvTimeout.tv_usec > 0){
    int retval = 0;
    int fcntlFlags = 0;
    if ((fcntlFlags = fcntl (this->sockFd, F_GETFL, NULL)) < 0) {
      return false;
    }
    if (fcntl (this->sockFd, F_SETFL, fcntlFlags | O_NONBLOCK) < 0) {
      return false;
    }
    if ((retval = connect (this->sockFd, (struct sockaddr *) &(this->addr), sizeof(this->addr))) < 0) {
      if (errno == EINPROGRESS) {
        fd_set waitSet;
        FD_ZERO (&waitSet);
        FD_SET (this->sockFd, &waitSet);
        retval = select (this->sockFd + 1, NULL, &waitSet, NULL, &(this->tvTimeout));
      }
    }
    else {
      retval = 1;
    }
    if (fcntl (this->sockFd, F_SETFL, fcntlFlags) < 0) {
      return false;
    }
    if (retval < 0) {
      return false;
    }
    else if (retval == 0) {
      errno = ETIMEDOUT;
      return false;
    }
    else {
      socklen_t len = sizeof (fcntlFlags);
      if (getsockopt (this->sockFd, SOL_SOCKET, SO_ERROR, &fcntlFlags, &len) < 0) {
        return false;
      }
      if (fcntlFlags) {
        errno = fcntlFlags;
        return false;
      }
    }
  }
  else {
    while (connect(this->sockFd, (struct sockaddr *) &(this->addr), sizeof(this->addr)) != 0) {
      usleep(10000); 
    }
  }
  return true;
}

/**
 * @brief Initialize TCPClient/IP Client connection.
 *
 * This function attempts to open the specified TCPClient/IP (socket) port for communication and connect it to the remote server.
 * It initialize the port according to the current settings and prepares it for data transfer.
 *
 * @return `0` Initialize process success.
 * @return `1` if the port fails to open.
 * @return `2` if failed to allocate connection parameters memory
 * @return `3` if failed to get hostname ip address
 * @return `4` if failed to create SSL_CTX (available if SSL layer mode is activated)
 * @return `5` if failed to use certificate (available if SSL layer mode is activated)
 * @return `6` if failed to use private key (available if SSL layer mode is activated)
 * @return `7` if failed to check priate key (available if SSL layer mode is activated)
 * @return `8` if failed to connect server
 * @return `9` if failed to connect server with SSL handshake (available if SSL layer mode is activated)
 */
int TCPClient::init(){
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  this->sockFd  = socket(AF_INET, SOCK_STREAM, 0);
  if (this->sockFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return 2;
  }
  this->addr.sin_family = AF_INET;
  if(this->isValidIPAddress(this->address) == false){
    struct hostent *host = NULL;
    host = gethostbyname((const char *) this->address.data());
    if (host != NULL){
      this->addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*) host->h_addr_list[0])));
    }
    else {
      close(this->sockFd);
      pthread_mutex_unlock(&(this->mtx));
      pthread_mutex_unlock(&(this->wmtx));
      return 3;
    }
  }
  else {
    memcpy(&(this->addr.sin_addr.s_addr), this->address.data(), 4);
  }
  if (this->tvTimeout.tv_sec > 0 || this->tvTimeout.tv_usec > 0){
    struct timeval tv;
    tv.tv_sec = this->tvTimeout.tv_sec;
    tv.tv_usec = this->tvTimeout.tv_usec;
    setsockopt(this->sockFd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &(tv), sizeof(tv));
  }
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_INITIALIZED);
  if (this->connectToServer() != true){
    close(this->sockFd);
    this->sockFd = -1;
    this->status = static_cast<unsigned char>(TCPClient::CLIENT_UNINITIALIZED);
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return 8;
  }
  this->status = static_cast<unsigned char>(TCPClient::CLIENT_CONNECTED);
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return 0;
}