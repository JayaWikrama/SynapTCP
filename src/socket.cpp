/*
 * $Id: socket.cpp,v 1.0.0 2024/09/10 22:31:12 Jaya Wikrama Exp $
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
#include "socket.hpp"

static void __TCP(Socket *obj){
  obj->setPort(3000);
  obj->setAddress("127.0.0.1");
  obj->setKeepAliveMs(0);
  obj->setTimeout(1, 500);
#ifdef __STCP_SSL__
  obj->setIsUseSSL(false);
  obj->setSSLVerifyMode(false);
#endif
}

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
Socket::Socket(){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
  __TCP(this);
  this->sockFd = -1;
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
}

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
Socket::Socket(const unsigned char *address) : Socket::Socket(){
  this->setAddress(address);
}

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
Socket::Socket(const unsigned char *address, int port) : Socket::Socket(){
  this->setAddress(address);
  this->setPort(port);
}

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
Socket::Socket(const std::vector <unsigned char> address) : Socket::Socket(){
  this->setAddress(address);
}

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
Socket::Socket(const std::vector <unsigned char> address, int port) : Socket::Socket(){
  this->setAddress(address);
  this->setPort(port);
}

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
Socket::Socket(const char *address) : Socket::Socket(){
  this->setAddress(address);
}

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
Socket::Socket(const char *address, int port) : Socket::Socket(){
  this->setAddress(address);
  this->setPort(port);
}

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
Socket::Socket(const std::string address) : Socket::Socket(){
  this->setAddress(address);
}

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
Socket::Socket(const std::string address, int port) : Socket::Socket(){
  this->setAddress(address);
  this->setPort(port);
}

/**
 * @brief Destructor.
 *
 * This destructor is responsible for releasing any memory that has been allocated during the object's lifetime.
 * It ensures that all allocated resources are properly freed, preventing memory leaks.
 */
Socket::~Socket(){
  this->closeSocket();
}

/**
 * @brief Check is IP Address valid or not.
 *
 * This method is responsible for validating whether the IP Address is valid or not.
 *
 * @param[in] address The IP Address (with a size of 4 bytes).
 * @return `true` when the IP Address is valid
 * @return `false` when the IP Address is invalid
 */
bool Socket::isValidIPAddress(const unsigned char *address){
  if (address[3] == 255){
    return false;
  }
  return true;
}

/**
 * @brief Overloaded method for `isValidIPAddress` to check is IP Address valid or not.
 *
 * This method is responsible for validating whether the IP Address is valid or not.
 *
 * @param[in] address The IP Address (with a size of 4 bytes).
 * @return `true` when the IP Address is valid
 * @return `false` when the IP Address is invalid
 */
bool Socket::isValidIPAddress(const std::vector <unsigned char> address){
  if (address.size() != 4) return false;
  return this->isValidIPAddress(address.data());
}

/**
 * @brief Overloaded method for `isValidIPAddress` to check is IP Address valid or not.
 *
 * This method is responsible for validating whether the IP Address is valid or not.
 *
 * @param[in] address The IP Address in string form.
 * @return `true` when the IP Address is valid
 * @return `false` when the IP Address is invalid
 */
bool Socket::isValidIPAddress(const char *address){
  if (strlen(address) > 15){
    return false;
  }
  if (*address == '.') return false;
  bool isPrevDot = false;
  unsigned short cnt = 0;
  unsigned short value = 0;
  do {
    if (*address == '.'){
      if (isPrevDot){
        return false;
      }
      value = 0;
      cnt++;
      isPrevDot = true;
    }
    else if (*address >= 0x30 && *address <= 0x39){
      value *= 10;
      value += static_cast<unsigned short>(*address - 0x30);
      if (value > 255){
        return false;
      }
      else if (cnt == 3 && value == 255){
        return false;
      }
      isPrevDot = false;
    }
    else {
      return false;
    }
    address++;
  } while (*address != 0x00);
  if (cnt > 3) return false;
  return true;
}

/**
 * @brief Overloaded method for `isValidIPAddress` to check is IP Address valid or not.
 *
 * This method is responsible for validating whether the IP Address is valid or not.
 *
 * @param[in] address The IP Address in string form.
 * @return `true` when the IP Address is valid
 * @return `false` when the IP Address is invalid
 */
bool Socket::isValidIPAddress(const std::string address){
  return this->isValidIPAddress(address.c_str());
}

/**
 * @brief Set Address of Socket Interface.
 *
 * This method is responsible for setting Address in IP Address form (with a size of 4 bytes).
 *
 * @param[in] address The IP Address (with a size of 4 bytes).
 * @return `true` when the IP Address is valid
 * @return `false` when the IP Address is invalid
 */
bool Socket::setAddress(const unsigned char *address){
  if (this->isValidIPAddress(address) == false) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->address.assign(address, address + 4);
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Overloaded method for `setAddress` to set Address of Socket Interface.
 *
 * This method is responsible for setting Address in IP Address form (with a size of 4 bytes).
 *
 * @param[in] address The IP Address (with a size of 4 bytes).
 * @return `true` when the IP Address is valid
 * @return `false` when the IP Address is invalid
 */
bool Socket::setAddress(const std::vector <unsigned char> address){
  if (this->isValidIPAddress(address) == false) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->address.assign(address.begin(), address.end());
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Overloaded method for `setAddress` to set Address of Socket Interface.
 *
 * This method is responsible for setting Address in IP Address or domain in string form.
 *
 * @param[in] address The IP Address (in string form).
 * @return `true` when the Address is valid (base on its pattern)
 * @return `false` when the Address is invalid
 */
bool Socket::setAddress(const std::string address){
  return this->setAddress(address.c_str());
}

/**
 * @brief Overloaded method for `setAddress` to set Address of Socket Interface.
 *
 * This method is responsible for setting Address in IP Address or domain in string form.
 *
 * @param[in] address The IP Address (in string form).
 * @return `true` when the Address is valid (base on its pattern)
 * @return `false` when the Address is invalid
 */
bool Socket::setAddress(const char *address){
  if (this->isValidIPAddress(address) == false) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  unsigned char tmp = 0x00;
  this->address.clear();
  do {
    if (*address == '.'){
      this->address.push_back(tmp);
      tmp = 0x00;
    }
    else {
      tmp = tmp * 10 + (*address - 0x30);
    }
    address++;
  } while (*address != 0x00);
  this->address.push_back(tmp);
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Set Socket port interface.
 *
 * This method is responsible for setting port of Socket interface.
 *
 * @param[in] port port communication.
 * @return `true` when the port is valid
 * @return `false` when the port is invalid
 */
bool Socket::setPort(int port){
  if (port <= 0 || port > 65535) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->addr.sin_port = (in_port_t) htons(port);
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Sets the communication timeout in milliseconds.
 *
 * This setter function configures the timeout for Socket communication. The timeout value is specified in units of 1 milliseconds.
 *
 * @param[in] milliseconds The timeout value.
 * @return `true` if the input value is valid.
 * @return `false` if the input value is invalid.
 */
bool Socket::setTimeout(int milliseconds){
  if (milliseconds < 0) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  if (milliseconds < 1000){
    this->tvTimeout.tv_usec = milliseconds * 1000;
    this->tvTimeout.tv_sec = 0;
  }
  else {
    this->tvTimeout.tv_sec = milliseconds / 1000;
    this->tvTimeout.tv_usec = (milliseconds % 1000) * 1000;
  }
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Overloaded method in `setTimeout` to sets the communication timeout in seconds and milliseconds.
 *
 * This setter function configures the timeout for Socket communication. The timeout value is specified in units of 1 seconds and 1 milliseconds.
 *
 * @param[in] seconds The timeout value in seconds.
 * @param[in] milliseconds The timeout value in milliseconds.
 * @return `true` if the input value is valid.
 * @return `false` if the input value is invalid.
 */
bool Socket::setTimeout(int seconds, int milliseconds){
  if (seconds < 0 || milliseconds < 0 || milliseconds > 999) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->tvTimeout.tv_sec = seconds;
  this->tvTimeout.tv_usec = milliseconds * 1000;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Sets the keep-alive interval for communication.
 *
 * This setter function configures the maximum wait time for receiving the next byte of data after the initial byte has been received. This helps maintain the connection by ensuring timely data reception.
 *
 * @param keepAliveMs The keep-alive interval in milliseconds.
 */
bool Socket::setKeepAliveMs(int keepAliveMs){
  if (keepAliveMs < 0) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->keepAliveMs = keepAliveMs;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Sets the SSL protection mode (is activated or not).
 *
 * This setter function configures the use of SSL handshake. This helps to improve the security of Socket communication.
 *
 * @param[in] useSSL The value true is assigned to enable SSL handshake.
 * @return `true` when success.
 * @return `false` when failed (if the SSL preprocessor is not enabled)
 */
bool Socket::setIsUseSSL(bool useSSL){
#ifdef __STCP_SSL__
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  if (this->useSSL == false || useSSL == false){
    this->sslVerifyMode = false;
  }
  this->useSSL = useSSL;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
#else
  return false;
#endif
}

/**
 * @brief Sets the SSL verify mode (is activated or not).
 *
 * This setter function configures the use of SSL verify mode.
 *
 * @param[in] sslVerifyMode The value true is assigned to enable SSL verify mode.
 * @return `true` when success.
 * @return `false` when failed (if the SSL preprocessor is not enabled or SSL handshake is not enabled)
 */
bool Socket::setSSLVerifyMode(bool sslVerifyMode){
#ifdef __STCP_SSL__
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  if (this->useSSL == false){
    this->sslVerifyMode = false;
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return false;
  }
  this->sslVerifyMode = sslVerifyMode;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
#else
  return false;
#endif
}

#ifdef __STCP_SSL__
/**
 * @brief Sets the SSL pointer.
 *
 * This setter function assign the SSL pointer.
 *
 * @param[in] sslConn pointer.
 * @return `true` when success.
 * @return `false` when failed (if the SSL preprocessor is not enabled)
 */
bool Socket::setSSLPointer(SSL *sslConn){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->sslConn = sslConn;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}
#endif

/**
 * @brief Manually setup the file descriptor.
 *
 * This setter function is used to manually setup or assign file descriptors.
 *
 * @param[in] sockFd The file descriptor
 * @return `true` for valid file descriptor
 * @return `false` for invalid file descriptor
 */
bool Socket::setSocketFd(int sockFd){
  if (sockFd <= 0 || sockFd >= 65535){
    return false;
  }
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->sockFd = sockFd;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Gets the address of Socket communication interface.
 *
 * This getter function retrieves address currently configured for Socket communication interface.
 *
 * @return The address of Socket Interface.
 */
std::string Socket::getAddress(){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  if (this->address.size() != 4){
    pthread_mutex_lock(&(this->mtx));
    pthread_mutex_lock(&(this->wmtx));
    return std::string("");
  }
  char result[16];
  memset(result, 0x00, sizeof(result));
  sprintf(result, "%d.%d.%d.%d",
          static_cast<int>(this->address[0]),
          static_cast<int>(this->address[1]),
          static_cast<int>(this->address[2]),
          static_cast<int>(this->address[3])
  );
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return std::string(result);
}

/**
 * @brief Gets the port for Socket communication interface.
 *
 * This getter function retrieves port that currently configured for Socket communication interface.
 *
 * @return The port for Socket communication interface.
 */
int Socket::getPort(){
  return ntohs(this->addr.sin_port);
}

/**
 * @brief Gets the communication timeout of Socket communication.
 *
 * This getter function retrieves the communication timeout that currently configured for Socket communication.
 *
 * @return The communication timeout.
 */
const struct timeval *Socket::getTimeout(){
  return &(this->tvTimeout);
}

/**
 * @brief Gets the keep-alive interval for communication.
 *
 * This getter function retrieves the maximum wait time configured for receiving the next byte of data after the initial byte has been successfully received. The interval is specified in milliseconds.
 *
 * @return The keep-alive interval in milliseconds.
 */
int Socket::getKeepAliveMs(){
  return this->keepAliveMs;
}

/**
 * @brief Gets is the SSL handshake active or not.
 *
 * This getter function retrieves is the SSL handshake active or not for Socket communication.
 *
 * @return SSL handshake (use) status.
 */
bool Socket::getIsUseSSL(){
#ifdef __STCP_SSL__
  return this->useSSL;
#else
  return false;
#endif
}

/**
 * @brief Gets is the SSL verify mode active or not.
 *
 * This getter function retrieves is the SSL verify mode active or not for Socket communication.
 *
 * @return SSL verify mode status.
 */
bool Socket::getSSLVerifyMode(){
#ifdef __STCP_SSL__
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  bool result = false;
  if (this->useSSL){
    result = this->sslVerifyMode;
  }
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return result;
#else
  return false;
#endif
}

/**
 * @brief Gets the file descriptor.
 *
 * This getter function is used to get the file descriptors.
 *
 * @return file descriptor.
 */
int Socket::getSocketFd(){
  return this->sockFd;
}

/**
 * @brief Checks whether socket communication has timed out based on its last activity.
 *
 * This function works by calculating the difference between the reference time and the socket's last activity time.
 * Then this value is compared with the timeout limit value of the socket.
 *
 * @param[in] ref The reference time.
 * @param[in] lastActivity The socket's last activity time.
 * @return `true` if timeout.
 * @return `false` if it hasn't timed out yet.
 */
bool Socket::isSocketTimeout(const struct timeval *ref, const struct timeval *lastActivity){
  static int diffTime = 0;
  static int timeoutInMs = 0;
  diffTime = ((ref->tv_sec - lastActivity->tv_sec) * 1000) + ((ref->tv_usec - lastActivity->tv_usec) / 1000);
  timeoutInMs = this->tvTimeout.tv_sec * 1000 + this->tvTimeout.tv_usec / 1000;
  if (diffTime > timeoutInMs){
    return true;
  }
  return false;
}

/**
 * @brief duplicate socket and its parameters.
 *
 * This function is responsible for duplicating all the parameters of its parent object except for SSL pointers.
 * Both objects use the same SSL pointer. So before deleting one of the objects, make sure to assign the SSL pointer
 * from one of the objects to the NULL value.
 *
 * @param[in] obj The target object.
 * @return `true` in success.
 * @return `false` if failed.
 */
bool Socket::duplicate(Socket &obj){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  pthread_mutex_lock(&(obj.mtx));
  pthread_mutex_lock(&(obj.wmtx));
  obj.address.assign(this->address.begin(), this->address.end());
  obj.sockFd = this->sockFd;
  obj.keepAliveMs = this->keepAliveMs;
  obj.len = this->len;
  memcpy(&(obj.tvTimeout), &(this->tvTimeout), sizeof(obj.tvTimeout));
  memcpy(&(obj.addr), &(this->addr), sizeof(obj.addr));
#ifdef __STCP_SSL__
  obj.useSSL = this->useSSL;
  obj.sslVerifyMode = this->sslVerifyMode;
  obj.sslConnRoutineOkStatus = this->sslConnRoutineOkStatus;
  obj.sslConn = this->sslConn;
#endif
  obj.data.assign(this->data.begin(), this->data.end());
  obj.remainingData.assign(this->remainingData.begin(), this->remainingData.end());
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  pthread_mutex_unlock(&(obj.mtx));
  pthread_mutex_unlock(&(obj.wmtx));
  return true;
}

/**
 * @brief Checks for available input bytes.
 *
 * This function checks whether there are any bytes available in the socket buffer.
 *
 * @return `true` if there are bytes available in the socket buffer.
 * @return `false` if there are no bytes available in the socket buffer.
 */
bool Socket::isInputBytesAvailable(){
  pthread_mutex_lock(&(this->mtx));
  long inputBytes = 0;
  if (ioctl(this->sockFd, FIONREAD, &inputBytes) != 0){
    pthread_mutex_unlock(&(this->mtx));
    return false;
  }
  pthread_mutex_unlock(&(this->mtx));
  return (inputBytes > 0 ? true : false);
}

/**
 * @brief Performs a Socket data receive operation.
 *
 * This function receives data from the Socket port without separating the successfully received data into the desired size and remaining data. The receive Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] sz The number of bytes to receive. A value of `0` means that the receiving operation is unlimited (up to the `keepAliveMs` timeout).
 * @param[in] dontSplitRemainingData A flag to disable automatic data splitting based on the amount of data requested.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveData(size_t sz, bool dontSplitRemainingData){
  pthread_mutex_lock(&(this->mtx));
  if (this->sockFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    return 1;
  }
  ssize_t bytes = 0;
  int idx = 0;
  size_t tmpSz = 0;
  unsigned char *tmp = nullptr;
  fd_set readfds;
  this->data.clear();
  struct timeval tvTmout;
  if (this->remainingData.size() > 0){
    this->data.assign(this->remainingData.begin(), this->remainingData.end());
    this->remainingData.clear();
  }
  FD_ZERO(&readfds);
  FD_SET(this->sockFd, &readfds);
  tvTmout.tv_sec = this->tvTimeout.tv_sec;
  tvTmout.tv_usec = this->tvTimeout.tv_usec;
  idx = select(this->sockFd + 1, &readfds, nullptr, nullptr, &tvTmout);
  if (idx <= 0){
    goto process;
  }
  else if (FD_ISSET(this->sockFd, &readfds)){
#ifdef __STCP_SSL__
    if (this->useSSL){
      tmpSz = 2048;
    }
    else {
      if (ioctl(this->sockFd, FIONREAD, &tmpSz) < 0){
        goto process;
      }
      if (tmpSz == 0){
        goto process;
      }
    }
#else
    if (ioctl(this->sockFd, FIONREAD, &tmpSz) < 0){
      goto process;
    }
    if (tmpSz == 0){
      goto process;
    }
#endif
    tmp = new unsigned char[tmpSz * sizeof(unsigned char)];
    if (tmp == nullptr){
      goto process;
    }
  }
  do {
    if (this->data.size() > 0) {
      if (this->keepAliveMs == 0) break;
      pthread_mutex_unlock(&(this->mtx));
      static struct timeval tvStart;
      static struct timeval tvEnd;
      static long diffTime = 0;
      gettimeofday(&tvStart, NULL);
      do {
        if (this->isInputBytesAvailable() == true){
          break;
        }
        else {
          usleep(1000);
        }
        gettimeofday(&tvEnd, NULL);
        diffTime = static_cast<long>((tvEnd.tv_sec - tvStart.tv_sec) * 1000) + static_cast<long>((tvEnd.tv_usec - tvStart.tv_usec) / 1000);
      } while (diffTime < static_cast<long>(this->keepAliveMs));
      if (this->isInputBytesAvailable() == false){
        pthread_mutex_lock(&(this->mtx));
        break;
      }
      pthread_mutex_lock(&(this->mtx));
    }
#ifdef __STCP_SSL__
    if (this->useSSL){
      if (this->sslConn == nullptr){
        pthread_mutex_unlock(&(this->wmtx));
        return 1;
      }
      bytes = SSL_read(this->sslConn, (void *) tmp, tmpSz);
    }
    else {
      bytes = read(this->sockFd, (void *) tmp, tmpSz);
    }
#else
    bytes = read(this->sockFd, (void *) tmp, tmpSz);
#endif
    if (bytes > 0){
      for (idx = 0; idx < bytes; idx++){
        this->data.push_back(tmp[idx]);
      }
    }
  } while (bytes > 0 && (sz == 0 || this->data.size() < sz));
  delete[] tmp;
  tmp = nullptr;
process:
  if (this->data.size() == 0){
    pthread_mutex_unlock(&(this->mtx));
    return 2;
  }
  if (dontSplitRemainingData == false && sz > 0 && this->data.size() > sz){
    this->remainingData.assign(this->data.begin() + sz, this->data.end());
    this->data.erase(this->data.begin() + sz, this->data.end());
  }
  pthread_mutex_unlock(&(this->mtx));
  return 0;
}

/**
 * @brief Overloaded method for `receiveData` to perform Socket data reception.
 *
 * This overloaded method performs a Socket data receiving operation.
 *
 * @param[in] sz The number of bytes to receive. A value of `0` means that the receiving operation is unlimited (up to the `keepAliveMs` timeout).
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveData(size_t sz){
  return this->receiveData(sz, false);
}

/**
 * @brief Overloaded method for `receiveData` to perform Socket data reception.
 *
 * This overloaded method performs a Socket data receiving operation.
 *
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveData(){
  return this->receiveData(0, false);
}

/**
 * @brief Recieves Socket data until the desired start bytes are found.
 *
 * This function performs a Socket data receiving operation until the specified start bytes are detected. Any Socket data received before the start bytes are found is automatically discarded. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] startBytes A pointer to the start bytes data to be detected.
 * @param[in] sz The size of the start bytes data to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveStartBytes(const unsigned char *startBytes, size_t sz){
  size_t i = 0;
  size_t idxCheck = 0;
  bool found = false;
  int ret = 0;
  std::vector <unsigned char> tmp;
  bool isRcvFirstBytes = false;
  do {
    if (this->remainingData.size() > 0){
      this->data.assign(this->remainingData.begin(), this->remainingData.end());
      this->remainingData.clear();
      ret = 0;
    }
    else {
      ret = this->receiveData(sz, true);
    }
    if (!ret){
      if (isRcvFirstBytes == false){
        isRcvFirstBytes = true;
      }
      else if (tmp.size() > sz){
        idxCheck = tmp.size() + 1 - sz;
      }
      tmp.insert(tmp.end(), this->data.begin(), this->data.end());
      if (this->remainingData.size() > 0){
        tmp.insert(tmp.end(), this->remainingData.begin(), this->remainingData.end());
        this->remainingData.clear();
      }
      if (tmp.size() >= sz){
        for (i = idxCheck; i <= tmp.size() - sz; i++){
          if (memcmp(tmp.data() + i, startBytes, sz) == 0){
            found = true;
            break;
          }
        }
      }
    }
  } while(found == false && ret == 0);
  if (found == true){
    this->data.clear();
    this->data.assign(tmp.begin() + i, tmp.begin() + i + sz);
    if (tmp.size() > i + sz) this->remainingData.assign(tmp.begin() + i + sz, tmp.end());
  }
  else {
    this->data.assign(tmp.begin(), tmp.end());
  }
  return ret;
}

/**
 * @brief Overloaded method for `receiveStartBytes` with `std::vector` input.
 *
 * This overloaded function performs a Socket data reception operation until the specified start bytes are detected. Any Socket data received before the start bytes are found is automatically discarded. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] startBytes A vector containing the start bytes data to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveStartBytes(const std::vector <unsigned char> startBytes){
  return this->receiveStartBytes(startBytes.data(), startBytes.size());
}

/**
 * @brief Overloaded method for `receiveStartBytes` with `const char*` input.
 *
 * This overloaded function performs a Socket data reception operation until the specified start bytes are detected. Any Socket data received before the start bytes are found is automatically discarded. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] startBytes A pointer to the start bytes data to be detected, provided as a `const char*`.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveStartBytes(const char *startBytes){
  return this->receiveStartBytes((const unsigned char *) startBytes, strlen(startBytes));
}

/**
 * @brief Overloaded method for `receiveStartBytes` with `std::string` input.
 *
 * This overloaded function performs a Socket data reception operation until the specified start bytes are detected. Any Socket data received before the start bytes are found is automatically discarded. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] startBytes A string containing the start bytes data to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveStartBytes(const std::string startBytes){
  return this->receiveStartBytes((const unsigned char *) startBytes.c_str(), startBytes.length());
}

/**
 * @brief Performs a Socket data receiving operation until the specified stop bytes are detected.
 *
 * This function receives Socket data until the specified stop bytes are detected. Any Socket data received up to and including the stop bytes is automatically stored in the buffer. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] stopBytes The data representing the stop bytes to be detected.
 * @param[in] sz The size of the stop bytes data to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveUntillStopBytes(const unsigned char *stopBytes, size_t sz){
  size_t i = 0;
  size_t idxCheck = 0;
  bool found = false;
  int ret = 0;
  std::vector <unsigned char> tmp;
  bool isRcvFirstBytes = false;
  do {
    if (this->remainingData.size() > 0){
      this->data.assign(this->remainingData.begin(), this->remainingData.end());
      this->remainingData.clear();
      ret = 0;
    }
    else {
      ret = this->receiveData(sz, true);
    }
    if (!ret){
      if (isRcvFirstBytes == false){
        isRcvFirstBytes = true;
      }
      else if (tmp.size() > sz){
        idxCheck = tmp.size() + 1 - sz;
      }
      tmp.insert(tmp.end(), this->data.begin(), this->data.end());
      if (this->remainingData.size() > 0){
        tmp.insert(tmp.end(), this->remainingData.begin(), this->remainingData.end());
        this->remainingData.clear();
      }
      if (tmp.size() >= sz){
        for (i = idxCheck; i <= tmp.size() - sz; i++){
          if (memcmp(tmp.data() + i, stopBytes, sz) == 0){
            found = true;
            break;
          }
        }
      }
    }
  } while(found == false && ret == 0);
  if (tmp.size() < sz){
    this->data.assign(tmp.begin(), tmp.end());
    return 2;
  }
  if (this->data.size() != tmp.size()){
    this->data.clear();
    this->data.assign(tmp.begin(), tmp.begin() + i + sz);
    if (tmp.size() > i + sz) this->remainingData.assign(tmp.begin() + i + sz, tmp.end());
    return 0;
  }
  if (this->data.size() > i + sz){
    this->remainingData.assign(this->data.begin() + i + sz, this->data.end());
    this->data.erase(this->data.begin() + i + sz, this->data.end());
  }
  return ret;
}

/**
 * @brief Overloaded function for `receiveUntillStopBytes` with input as `std::vector`.
 *
 * This function receives Socket data until the specified stop bytes are detected. Any Socket data received up to and including the stop bytes is automatically stored in the buffer. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] stopBytes A vector of `unsigned char` representing the stop bytes to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveUntillStopBytes(const std::vector <unsigned char> stopBytes){
  return this->receiveUntillStopBytes(stopBytes.data(), stopBytes.size());
}

/**
 * @brief Overloaded function for `receiveUntillStopBytes` with input as `const char*`.
 *
 * This function receives Socket data until the specified stop bytes are detected. Any Socket data received up to and including the stop bytes is automatically stored in the buffer. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] stopBytes A pointer to a null-terminated character array representing the stop bytes to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveUntillStopBytes(const char *stopBytes){
  return this->receiveUntillStopBytes((const unsigned char *) stopBytes, strlen(stopBytes));
}

/**
 * @brief Overloaded function for `receiveUntillStopBytes` with input as `std::string`.
 *
 * This function receives Socket data until the specified stop bytes are detected. Any Socket data received up to and including the stop bytes is automatically stored in the buffer. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] stopBytes A string representing the stop bytes to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveUntillStopBytes(const std::string stopBytes){
  return this->receiveUntillStopBytes((const unsigned char *) stopBytes.c_str(), stopBytes.length());
}

/**
 * @brief Receive Socket data and checks if the data matches the specified stop bytes.
 *
 * This method performs Socket data reception while simultaneously checking if the data matches the desired stop bytes. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] stopBytes A pointer to the stop bytes data to be detected.
 * @param[in] sz The size of the stop bytes data to be detected.
 * @return `0` if the operation is successful and the data is valid.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 * @return `3` if data is recieved but does not match the specified stop bytes.
 */
int Socket::receiveStopBytes(const unsigned char *stopBytes, size_t sz){
  bool found = false;
  int ret = 0;
  std::vector <unsigned char> tmp;
  do {
    if (this->remainingData.size() > 0){
      this->data.assign(this->remainingData.begin(), this->remainingData.end());
      this->remainingData.clear();
      ret = 0;
    }
    else {
      ret = this->receiveData(sz - tmp.size(), true);
    }
    if (!ret){
      tmp.insert(tmp.end(), this->data.begin(), this->data.end());
      if (this->remainingData.size() > 0){
        tmp.insert(tmp.end(), this->remainingData.begin(), this->remainingData.end());
        this->remainingData.clear();
      }
      if (tmp.size() >= sz){
        if (memcmp(tmp.data(), stopBytes, sz) == 0){
          found = true;
        }
        break;
      }
    }
  } while(ret == 0);
  if (tmp.size() < sz){
    this->data.assign(tmp.begin(), tmp.end());
    return 2;
  }
  if (found == false){
    this->data.assign(tmp.begin(), tmp.end());
    return 3;
  }
  if (this->data.size() != tmp.size()){
    this->data.clear();
    this->data.assign(tmp.begin(), tmp.begin() + sz);
    if (tmp.size() > sz) this->remainingData.assign(tmp.begin() + sz, tmp.end());
    return 0;
  }
  if (this->data.size() > sz){
    this->remainingData.assign(this->data.begin() + sz, this->data.end());
    this->data.erase(this->data.begin() + sz, this->data.end());
  }
  return ret;
}

/**
 * @brief Function overloading for `receiveStopBytes` with input using `std::vector`.
 *
 * This method performs Socket data reception while simultaneously checking if the data matches the desired stop bytes. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] stopBytes A `std::vector` containing the stop bytes data to be detected.
 * @return `0` if the operation is successful and the data is valid.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 * @return `3` if data is received but does not match the specified stop bytes.
 */
int Socket::receiveStopBytes(const std::vector <unsigned char> stopBytes){
  return this->receiveStopBytes(stopBytes.data(), stopBytes.size());
}

/**
 * @brief Function overloading for `receiveStopBytes` with input using `char *`.
 *
 * This method performs Socket data reception while simultaneously checking if the data matches the desired stop bytes. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] stopBytes A C-style string (null-terminated) representing the stop bytes to be detected.
 * @return `0` if the operation is successful and the data is valid.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 * @return `3` if data is received but does not match the specified stop bytes.
 */
int Socket::receiveStopBytes(const char *stopBytes){
  return this->receiveStopBytes((const unsigned char *) stopBytes, strlen(stopBytes));
}

/**
 * @brief Function overloading for `receiveStopBytes` with input using `std::string`.
 *
 * This method performs Socket data reception while simultaneously checking if the data matches the desired stop bytes. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] stopBytes A `std::string` object representing the stop bytes to be detected.
 * @return `0` if the operation is successful and the data is valid.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 * @return `3` if data is received but does not match the specified stop bytes.
 */
int Socket::receiveStopBytes(const std::string stopBytes){
  return this->receiveStopBytes((const unsigned char *) stopBytes.c_str(), stopBytes.length());
}

/**
 * @brief Performs Socket data reception until the desired amount of data is received.
 *
 * This function performs the operation of receiving Socket data until the specified amount of data is fulfilled. The operation retries up to 3 times, starting from the first data received. The received Socket data can be accessed using the `Socket::getBuffer` method.
 *
 * @param[in] sz The size of the Socket data to be received.
 * @return `0` if successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int Socket::receiveNBytes(size_t sz){
  size_t i = 0;
  std::vector <unsigned char> tmp;
  int ret = 0;
  int tryTimes = 0;
  bool isRcvFirstBytes = false;
  do {
    if (this->remainingData.size() > 0){
      this->data.assign(this->remainingData.begin(), this->remainingData.end());
      this->remainingData.clear();
      ret = 0;
    }
    else {
      ret = this->receiveData(sz, true);
    }
    if (!ret){
      if (isRcvFirstBytes == false){
        tryTimes = 3;
        isRcvFirstBytes = true;
      }
      tmp.insert(tmp.end(), this->data.begin(), this->data.end());
      if (this->remainingData.size() > 0){
        tmp.insert(tmp.end(), this->remainingData.begin(), this->remainingData.end());
        this->remainingData.clear();
      }
      if (tmp.size() >= sz) break;
    }
    else if (isRcvFirstBytes == true) {
      tryTimes--;
    }
  } while(tryTimes > 0);
  if (tmp.size() < sz){
    this->data.assign(tmp.begin(), tmp.end());
    return 2;
  }
  if (this->data.size() != tmp.size()){
    this->data.clear();
    this->data.assign(tmp.begin(), tmp.begin() + sz);
    if (tmp.size() > sz) this->remainingData.assign(tmp.begin() + sz, tmp.end());
    return 0;
  }
  if (this->data.size() > sz){
    this->remainingData.assign(this->data.begin() + sz, this->data.end());
    this->data.erase(this->data.begin() + sz, this->data.end());
  }
  return 0;
}

/**
 * @brief Retrieves the amount of successfully received data.
 *
 * This function retrieves the information about the size of the data that has been successfully received.
 *
 * @return The size of the data in bytes.
 */
size_t Socket::getDataSize(){
  return this->data.size();
}

/**
 * @brief Retrieves the received data buffer.
 *
 * This function retrieves all the data that has been successfully received by the `receive` method.
 *
 * @param[out] buffer A variable to hold the data that has been successfully received.
 * @param[in] maxBufferSz The maximum size of the data that can be accommodated in the buffer.
 * @return The size of the data received.
 */
size_t Socket::getBuffer(unsigned char *buffer, size_t maxBufferSz){
  pthread_mutex_lock(&(this->mtx));
  size_t result = (this->data.size() < maxBufferSz ? this->data.size() : maxBufferSz);
  memset(buffer, 0x00, maxBufferSz);
  if (result > 0) memcpy(buffer, this->data.data(), result);
  pthread_mutex_unlock(&(this->mtx));
  return result;
}

/**
 * @brief Overloaded method for `getBuffer` with `std::vector<unsigned char>` as the output parameter.
 *
 * Retrieves all the data that has been successfully received by the `receive` method.
 *
 * This overload allows you to use a `std::vector<unsigned char>` as the buffer to hold the received data.
 *
 * @param[out] buffer A `std::vector<unsigned char>` to hold the received data that has been successfully received.
 * @return The size of the data received.
 */
size_t Socket::getBuffer(std::vector <unsigned char> &buffer){
  pthread_mutex_lock(&(this->mtx));
  buffer.clear();
  buffer.assign(this->data.begin(), this->data.end());
  pthread_mutex_unlock(&(this->mtx));
  return buffer.size();
}

/**
 * @brief Retrieves the received data buffer as a vector.
 *
 * This method returns all the data that has been successfully received by the `receive` method as a `std::vector<unsigned char>`.
 *
 * @return A `std::vector<unsigned char>` containing the data that has been successfully recieved.
 */
std::vector <unsigned char> Socket::getBufferAsVector(){
  pthread_mutex_lock(&(this->mtx));
  std::vector <unsigned char> tmp;
  tmp.assign(this->data.begin(), this->data.end());
  pthread_mutex_unlock(&(this->mtx));
  return tmp;
}

/**
 * @brief Retrieves the number of bytes in the remaining buffer.
 *
 * This method provides the size of the data that is still present in the remaining buffer.
 *
 * @return The size of the remaining data in bytes.
 */
size_t Socket::getRemainingDataSize(){
  return this->remainingData.size();
}

/**
 * @brief Retrieves the remaining recieved data outside of the data buffer.
 *
 * This method extracts all remaining data that has been successfully received but is outside the main data buffer.
 *
 * @param[out] buffer Variable to hold the remaining recieved data.
 * @param[in] maxBufferSz Maximum size of data that can be held by the buffer variable.
 * @return The size of the data.
 */
size_t Socket::getRemainingBuffer(unsigned char *buffer, size_t maxBufferSz){
  pthread_mutex_lock(&(this->mtx));
  size_t result = (this->remainingData.size() < maxBufferSz ? this->remainingData.size() : maxBufferSz);
  memcpy(buffer, this->remainingData.data(), result);
  pthread_mutex_unlock(&(this->mtx));
  return result;
}

/**
 * @brief Overloading method for __getRemainingBuffer__ with output parameter as vector.
 *
 * Retrieves all remaining data that has been successfully received but is outside the main data buffer.
 *
 * @param[out] buffer Variable to hold the remaining data, provided as a vector.
 * @return The size of the remaining data.
 */
size_t Socket::getRemainingBuffer(std::vector <unsigned char> &buffer){
  pthread_mutex_lock(&(this->mtx));
  buffer.clear();
  buffer.assign(this->remainingData.begin(), this->remainingData.end());
  pthread_mutex_unlock(&(this->mtx));
  return this->remainingData.size();
}

/**
 * @brief Retrieves remaining data that has been successfully received but is outside the main data buffer, returning it as a vector.
 *
 * This method retrieves all remaining data that has been successfully received but is not included in the main data buffer, and returns it as a vector.
 *
 * @return std::vector<unsigned char> containing the remaining data that has been successfully received.
 */
std::vector <unsigned char> Socket::getRemainingBufferAsVector(){
  pthread_mutex_lock(&(this->mtx));
  std::vector <unsigned char> tmp;
  tmp.assign(this->remainingData.begin(), this->remainingData.end());
  pthread_mutex_unlock(&(this->mtx));
  return tmp;
}

/**
 * @brief Performs the operation of sending Socket data.
 *
 * This method sends the specified data to the Socket port.
 *
 * @param[in] buffer Data to be written.
 * @param[in] sz Size of the data to be written.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if the data write operation fails.
 */
int Socket::sendData(const unsigned char *buffer, size_t sz){
  pthread_mutex_lock(&(this->wmtx));
  size_t total = 0;
  struct timeval tv_start;
  struct timeval tv_crn;
  int diffTime = 0;
  if (this->sockFd <= 0){
    pthread_mutex_unlock(&(this->wmtx));
    return 1;
  }
  ssize_t bytes = 0;
  /* check Socket Output Buffer before send any data */
  gettimeofday(&tv_start, NULL);
  do {
    ioctl(this->sockFd, TIOCOUTQ, &bytes);
    if (bytes > 0){
      gettimeofday(&tv_crn, NULL);
      diffTime = (tv_crn.tv_sec - tv_start.tv_sec) * 1000 + (tv_crn.tv_usec - tv_start.tv_usec) / 1000;
      if (this->tvTimeout.tv_sec > 0 || this->tvTimeout.tv_usec > 0){
        if (diffTime > (this->tvTimeout.tv_sec * 1000 + this->tvTimeout.tv_usec / 1000)){
          pthread_mutex_unlock(&(this->wmtx));
          return 2;
        }
      }
      else {
        pthread_mutex_unlock(&(this->wmtx));
        return 2;
      }
    }
  } while (bytes > 0);
#ifdef __STCP_SSL__
  if (this->useSSL){
    if (this->sslConn == nullptr){
      pthread_mutex_unlock(&(this->wmtx));
      return 1;
    }
    if (BIO_ctrl_pending(SSL_get_wbio(this->sslConn)) != 0){
      pthread_mutex_unlock(&(this->wmtx));
      return 2;
    }
  }
#endif
  bytes = 0;
    /* send data */
  while (total < sz){
#ifdef __STCP_SSL__
    if (this->useSSL){
      if (this->sslConn == nullptr){
        pthread_mutex_unlock(&(this->wmtx));
        return 1;
      }
      bytes = SSL_write(this->sslConn, (void *) (buffer + total), sz - total);
    }
    else {
      bytes = write(this->sockFd, (void *) (buffer + total), sz - total);
    }
#else
    bytes = write(this->sockFd, (void *) (buffer + total), sz - total);
#endif
    if (bytes > 0){
      total += bytes;
    }
    else {
      pthread_mutex_unlock(&(this->wmtx));
      return 2;
    }
  }
  pthread_mutex_unlock(&(this->wmtx));
  return 0;
}

/**
 * @brief Method overloading of `sendData` with input as `const std::vector`.
 *
 * This method sends the specified data to the Socket port. This overload allows the data to be passed as a `const std::vector`.
 *
 * @param[in] buffer Data to be written.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if the data write operation fails.
 */
int Socket::sendData(const std::vector <unsigned char> buffer){
  return this->sendData(buffer.data(), buffer.size());
}

/**
 * @brief Method overloading of `sendData` with input as `const char*`.
 *
 * This method sends the specified data to the Socket port. This overload allows the data to be passed as a `const char*`.
 *
 * @param[in] buffer Data to be written.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if the data write operation fails.
 */
int Socket::sendData(const char *buffer){
  return this->sendData((const unsigned char *) buffer, strlen(buffer));
}

/**
 * @brief Method overloading of `sendData` with input as `const std::string`.
 *
 * This method sends the specified data to the Socket port. This overload allows the data to be passed as a `const std::string`.
 *
 * @param[in] buffer Data to be written.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if the data write operation fails.
 */
int Socket::sendData(const std::string buffer){
  return this->sendData((const unsigned char *) buffer.c_str(), buffer.length());
}

/**
 * @brief Closes the Socket connection between client and server.
 *
 * This function is used to close the currently Socket connection between client and server,
 * ensuring that the port is no longer in use and that any associated system resources
 * are released.
 */
void Socket::closeConnection(){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
#ifdef __STCP_SSL__
  if (this->sslConn != nullptr && this->useSSL){
    if (this->sockFd > 0){
      if (!SSL_get_shutdown(this->sslConn)){
        SSL_shutdown(this->sslConn);
      }
    }
    else {
      if (this->sslConnRoutineOkStatus != -1){
        SSL_shutdown(this->sslConn);
      }
    }
    SSL_free(this->sslConn);
    this->sslConn = nullptr;
  }
  this->sslConnRoutineOkStatus = 0;
#endif
  if (this->sockFd > 0){
    if (!close(this->sockFd)){
      this->sockFd = -1;
    }
  }
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
}

/**
 * @brief Closes the Socket communication port.
 *
 * This function is used to close the currently Socket communication port,
 * ensuring that the port is no longer in use and that any associated system resources
 * are released.
 */
void Socket::closeSocket(){
  this->closeConnection();
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  if (this->sockFd > 0){
    if (!close(this->sockFd)){
      this->sockFd = -1;
    }
  }
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
}