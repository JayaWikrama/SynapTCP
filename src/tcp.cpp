#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "tcp.hpp"

static void __TCP(TCP *obj){
  obj->setMaximumClient(10);
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
 * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
 * - `port = 3000` : Port is set to 3000 as its default value.
 * - `address = 127.0.0.1` : Default address is set to localhost.
 * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
 * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
 * - Initializes the mutex for thread safety.
 */
TCP::TCP(){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
TCP::TCP(const unsigned char *address){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->setAddress(address);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
 * @param[in] port The port of TCP/IP interface.
 */
TCP::TCP(const unsigned char *address, int port){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->setAddress(address);
  this->setPort(port);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
TCP::TCP(const std::vector <unsigned char> address){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->setAddress(address);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
 * @param[in] port The port of TCP/IP interface.
 */
TCP::TCP(const std::vector <unsigned char> address, int port){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->setAddress(address);
  this->setPort(port);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
TCP::TCP(const char *address){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->setAddress(address);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
 * @param[in] port The port of TCP/IP interface.
 */
TCP::TCP(const char *address, int port){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->setAddress(address);
  this->setPort(port);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
TCP::TCP(const std::string address){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->setAddress(address);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
 * @param[in] port The port of TCP/IP interface.
 */
TCP::TCP(const std::string address, int port){
  pthread_mutex_init(&(this->mtx), nullptr);
  pthread_mutex_init(&(this->wmtx), nullptr);
  __TCP(this);
  this->setAddress(address);
  this->setPort(port);
  this->sockFd = -1;
  this->connFd = -1;
  memset(&(this->len), 0x00, sizeof(this->len));
  memset(&(this->addr), 0x00, sizeof(this->addr));
#ifdef __STCP_SSL__
  this->sslConnRoutineOkStatus = 0;
  this->sslConn = nullptr;
  this->sslCtx = nullptr;
#endif
  this->data.clear();
  this->remainingData.clear();
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
bool TCP::isValidIPAddress(const unsigned char *address){
  if (address[0] == 255 &&
      address[1] == 255 &&
      address[2] == 255 &&
      address[3] == 255
  ){
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
bool TCP::isValidIPAddress(const std::vector <unsigned char> address){
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
bool TCP::isValidIPAddress(const char *address){
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
      if (value >= 255){
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
bool TCP::isValidIPAddress(const std::string address){
  return this->isValidIPAddress(address.c_str());
}

/**
 * @brief Set Address of TCP/IP Interface.
 *
 * This method is responsible for setting Address in IP Address form (with a size of 4 bytes).
 *
 * @param[in] address The IP Address (with a size of 4 bytes).
 * @return `true` when the IP Address is valid
 * @return `false` when the IP Address is invalid
 */
bool TCP::setAddress(const unsigned char *address){
  if (this->isValidIPAddress(address) == false) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->address.assign(address, address + 4);
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Overloaded method for `setAddress` to set Address of TCP/IP Interface.
 *
 * This method is responsible for setting Address in IP Address form (with a size of 4 bytes).
 *
 * @param[in] address The IP Address (with a size of 4 bytes).
 * @return `true` when the IP Address is valid
 * @return `false` when the IP Address is invalid
 */
bool TCP::setAddress(const std::vector <unsigned char> address){
  if (this->isValidIPAddress(address) == false) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->address.assign(address.begin(), address.end());
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Overloaded method for `setAddress` to set Address of TCP/IP Interface.
 *
 * This method is responsible for setting Address in IP Address or domain in string form.
 *
 * @param[in] address The IP Address (in string form).
 * @return `true` when the Address is valid (base on its pattern)
 * @return `false` when the Address is invalid
 */
bool TCP::setAddress(const std::string address){
  return this->setAddress(address.c_str());
}

/**
 * @brief Overloaded method for `setAddress` to set Address of TCP/IP Interface.
 *
 * This method is responsible for setting Address in IP Address or domain in string form.
 *
 * @param[in] address The IP Address (in string form).
 * @return `true` when the Address is valid (base on its pattern)
 * @return `false` when the Address is invalid
 */
bool TCP::setAddress(const char *address){
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
 * @brief Set TCP/IP port interface.
 *
 * This method is responsible for setting port of TCP/IP interface.
 *
 * @param[in] port port communication.
 * @return `true` when the port is valid
 * @return `false` when the port is invalid
 */
bool TCP::setPort(int port){
  if (port < 0 && port > 65535) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->port = port;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Set the maximum clients that the TCP/IP Server can handle.
 *
 * This method is responsible for setting the maximum number of clients that the TCP/IP Server can handle. This method only for server side.
 *
 * @param[in] nClient the maximum number of clients.
 * @return `true` when the client number is valid
 * @return `false` when the client number is invalid
 */
bool TCP::setMaximumClient(int nClient){
  if (nClient < 1 || nClient > 65535) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->maxClient = nClient;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Overloaded method in `setTimeout` to sets the communication timeout in milliseconds.
 *
 * This setter function configures the timeout for TCP/IP communication. The timeout value is specified in units of 1 milliseconds.
 *
 * @param[in] milliseconds The timeout value.
 * @return `true` if the input value is valid.
 * @return `false` if the input value is invalid.
 */
bool TCP::setTimeout(int milliseconds){
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
 * This setter function configures the timeout for TCP/IP communication. The timeout value is specified in units of 1 seconds and 1 milliseconds.
 *
 * @param[in] seconds The timeout value in seconds.
 * @param[in] milliseconds The timeout value in milliseconds.
 * @return `true` if the input value is valid.
 * @return `false` if the input value is invalid.
 */
bool TCP::setTimeout(int seconds, int milliseconds){
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
bool TCP::setKeepAliveMs(int keepAliveMs){
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
 * This setter function configures the use of SSL handshake. This helps to improve the security of TCP/IP communication.
 *
 * @param[in] isUseSSL The value true is assigned to enable SSL handshake.
 * @return `true` when success.
 * @return `false` when failed (if the SSL preprocessor is not enabled)
 */
bool TCP::setIsUseSSL(bool isUseSSL){
#ifdef __STCP_SSL__
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  if (this->useSSL == false || isUseSSL == false){
    this->sslVerifyMode = false;
  }
  this->useSSL = isUseSSL;
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
bool TCP::setSSLVerifyMode(bool sslVerifyMode){
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

/**
 * @brief Sets the SSL CTX pointer.
 *
 * This setter function assign the SSL CTX pointer.
 *
 * @param[in] sslCtx pointer.
 * @return `true` when success.
 * @return `false` when failed (if the SSL preprocessor is not enabled)
 */
bool TCP::setSSLCTXPointer(SSL_CTX *sslCtx){
#ifdef __STCP_SSL__
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->sslCtx = sslCtx;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
#else
  return false;
#endif
}

/**
 * @brief Sets the SSL pointer.
 *
 * This setter function assign the SSL pointer.
 *
 * @param[in] sslConn pointer.
 * @return `true` when success.
 * @return `false` when failed (if the SSL preprocessor is not enabled)
 */
bool TCP::setSSLPointer(SSL *sslConn){
#ifdef __STCP_SSL__
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->sslConn = sslConn;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
#else
  return false;
#endif
}

/**
 * @brief Gets the address of TCP/IP communication interface.
 *
 * This getter function retrieves address currently configured for TCP/IP communication interface.
 *
 * @return The address of TCP/IP Interface.
 */
std::string TCP::getAddress(){
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
 * @brief Gets the port for TCP/IP communication interface.
 *
 * This getter function retrieves port that currently configured for TCP/IP communication interface.
 *
 * @return The port for TCP/IP communication interface.
 */
int TCP::getPort(){
  return this->port;
}

/**
 * @brief Gets the maximum clients that the TCP/IP Server can handle.
 *
 * This getter function retrieves the maximum clients (the TCP/IP Server can handle) that currently configured for TCP/IP communication interface.
 *
 * @return The number of client that the TCP/IP Server can handle.
 */
int TCP::getMaximumClient(){
  return this->maxClient;
}

/**
 * @brief Gets the communication timeout of TCP/IP communication.
 *
 * This getter function retrieves the communication timeout that currently configured for TCP/IP communication.
 *
 * @return The communication timeout.
 */
const struct timeval *TCP::getTimeout(){
  return &(this->tvTimeout);
}

/**
 * @brief Gets the keep-alive interval for communication.
 *
 * This getter function retrieves the maximum wait time configured for receiving the next byte of data after the initial byte has been successfully received. The interval is specified in milliseconds.
 *
 * @return The keep-alive interval in milliseconds.
 */
int TCP::getKeepAliveMs(){
  return this->keepAliveMs;
}

/**
 * @brief Gets is the SSL handshake active or not.
 *
 * This getter function retrieves is the SSL handshake active or not for TCP/IP communication.
 *
 * @return SSL handshake (use) status.
 */
bool TCP::getIsUseSSL(){
#ifdef __STCP_SSL__
  return this->useSSL;
#else
  return false;
#endif
}

/**
 * @brief Gets is the SSL verify mode active or not.
 *
 * This getter function retrieves is the SSL verify mode active or not for TCP/IP communication.
 *
 * @return SSL verify mode status.
 */
bool TCP::getSSLVerifyMode(){
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
 * @brief Initialize TCP/IP Server connection.
 *
 * This function attempts to open the specified TCP/IP (socket) port for communication. It initialize the port according to the current settings and prepares it for data transfer.
 *
 * @return `0` Initialize process success.
 * @return `1` if the port fails to open.
 * @return `2` if failed to allocate connection parameters memory.
 * @return `3` if failed to bind connection
 * @return `4` if failed to listen connection
 * @return `5` if failed to create SSL_CTX (available if SSL layer mode is activated)
 * @return `6` if failed to use certificate (available if SSL layer mode is activated)
 * @return `7` if failed to use private key (available if SSL layer mode is activated)
 * @return `8` if failed to check priate key (available if SSL layer mode is activated)
 * @return `9` if failed to load verify location (available if SSL layer mode is activated)
 * @return `10` if failed to load cacert (available if SSL layer mode is activated)
 */
int TCP::serverInit(){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  int retval = 0;
  const int optVal = 1;
  const socklen_t optLen = sizeof(optVal);
  this->sockFd  = socket(AF_INET, SOCK_STREAM, 0);
  if (this->sockFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return 1;
  }
  memset(&(this->addr), 0x00, sizeof(this->addr));
  this->addr.sin_family = AF_INET;
  this->addr.sin_port = (in_port_t) htons(this->port);
  memcpy(&(this->addr.sin_addr.s_addr), this->address.data(), 4);
  setsockopt(this->sockFd, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
  if (bind(this->sockFd, (const struct sockaddr *) &(this->addr), sizeof(this->addr))){
    close (this->sockFd);
    this->sockFd = -1;
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return 3;
  }
  if (listen(this->sockFd, this->maxClient)){
    close (this->sockFd);
    this->sockFd = -1;
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return 4;
  }
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return 0;
}

/**
 * @brief Accept the available client when TCP/IP Server listen the connection.
 *
 * This function attempts to accept the connection that requested by client side.
 *
 * @return `true` in success.
 * @return `false` if failed.
 */
bool TCP::serverAccept(){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->len = (socklen_t) sizeof(this->addr);
  this->connFd = accept(this->sockFd, (struct sockaddr *) &(this->addr), &(this->len));
  if (this->connFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return false;
  }
  if (this->tvTimeout.tv_sec > 0 ||  this->tvTimeout.tv_usec){
    struct timeval tv;
    tv.tv_sec = this->tvTimeout.tv_sec;
    tv.tv_usec = this->tvTimeout.tv_usec;
    setsockopt(this->connFd, SOL_SOCKET, SO_RCVTIMEO, (const unsigned char *) &tv, sizeof(tv));
  }
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Connect to TCP/IP Server with/without timeout.
 *
 * This method is called automatically when the `clientInit` method is called.
 *
 * @return `true` if success
 * @return `false` if fail or timeout
 */
bool TCP::cliConnect(){
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
 * @brief Initialize TCP/IP Client connection.
 *
 * This function attempts to open the specified TCP/IP (socket) port for communication. It initialize the port according to the current settings and prepares it for data transfer.
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
int TCP::clientInit(){
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  this->sockFd  = socket(AF_INET, SOCK_STREAM, 0);
  if (this->sockFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return 2;
  }
  memset(&(this->addr), 0x00, sizeof(this->addr));
  this->addr.sin_family = AF_INET;
  this->addr.sin_port = (in_port_t) htons(this->port);
  if(this->isValidIPAddress(this->address) != 0){
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
  if (this->cliConnect() != true){
    close(this->sockFd);
    this->sockFd = -1;
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return 8;
  }
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return 0;
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
bool TCP::duplicate(TCP &obj){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  pthread_mutex_lock(&(obj.mtx));
  pthread_mutex_lock(&(obj.wmtx));
  obj.maxClient = this->maxClient;
  obj.port = this->port;
  obj.address.assign(this->address.begin(), this->address.end());
  obj.sockFd = this->sockFd;
  obj.connFd = this->connFd;
  obj.keepAliveMs = this->keepAliveMs;
  obj.len = this->len;
  memcpy(&(obj.tvTimeout), &(this->tvTimeout), sizeof(obj.tvTimeout));
  memcpy(&(obj.addr), &(this->addr), sizeof(obj.addr));
#ifdef __STCP_SSL__
  obj.useSSL = this->useSSL;
  obj.sslVerifyMode = this->sslVerifyMode;
  obj.sslConnRoutineOkStatus = this->sslConnRoutineOkStatus;
  obj.sslCtx = this->sslCtx;
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
 * @brief Performs a TCP/IP data receive operation.
 *
 * This function receives data from the TCP/IP port without separating the successfully received data into the desired size and remaining data. The receive TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] sz The number of bytes to receive. A value of `0` means that the receiving operation is unlimited (up to the `keepAliveMs` timeout).
 * @param[in] dontSplitRemainingData A flag to disable automatic data splitting based on the amount of data requested.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveData(size_t sz, bool dontSplitRemainingData){
  pthread_mutex_lock(&(this->mtx));
  if (this->connFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    return 1;
  }
  ssize_t bytes = 0;
  int idx = 0;
  unsigned char tmp[128];
  this->data.clear();
  if (this->remainingData.size() > 0){
    this->data.assign(this->remainingData.begin(), this->remainingData.end());
    this->remainingData.clear();
  }
  do {
    bytes = read(this->connFd, (void *) tmp, sizeof(tmp));
    if (bytes > 0){
      for (idx = 0; idx < bytes; idx++){
        this->data.push_back(tmp[idx]);
      }
    }
  } while (bytes > 0 && sz == 0);
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
 * @brief Overloaded method for `receiveData` to perform TCP/IP data reception.
 *
 * This overloaded method performs a TCP/IP data receiving operation.
 *
 * @param[in] sz The number of bytes to receive. A value of `0` means that the receiving operation is unlimited (up to the `keepAliveMs` timeout).
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveData(size_t sz){
  return this->receiveData(sz, false);
}

/**
 * @brief Overloaded method for `receiveData` to perform TCP/IP data reception.
 *
 * This overloaded method performs a TCP/IP data receiving operation.
 *
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveData(){
  return this->receiveData(0, false);
}

/**
 * @brief Recieves TCP/IP data until the desired start bytes are found.
 *
 * This function performs a TCP/IP data receiving operation until the specified start bytes are detected. Any TCP/IP data received before the start bytes are found is automatically discarded. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] startBytes A pointer to the start bytes data to be detected.
 * @param[in] sz The size of the start bytes data to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveStartBytes(const unsigned char *startBytes, size_t sz){
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
 * This overloaded function performs a TCP/IP data reception operation until the specified start bytes are detected. Any TCP/IP data received before the start bytes are found is automatically discarded. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] startBytes A vector containing the start bytes data to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveStartBytes(const std::vector <unsigned char> startBytes){
  return this->receiveStartBytes(startBytes.data(), startBytes.size());
}

/**
 * @brief Overloaded method for `receiveStartBytes` with `const char*` input.
 *
 * This overloaded function performs a TCP/IP data reception operation until the specified start bytes are detected. Any TCP/IP data received before the start bytes are found is automatically discarded. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] startBytes A pointer to the start bytes data to be detected, provided as a `const char*`.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveStartBytes(const char *startBytes){
  return this->receiveStartBytes((const unsigned char *) startBytes, strlen(startBytes));
}

/**
 * @brief Overloaded method for `receiveStartBytes` with `std::string` input.
 *
 * This overloaded function performs a TCP/IP data reception operation until the specified start bytes are detected. Any TCP/IP data received before the start bytes are found is automatically discarded. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] startBytes A string containing the start bytes data to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveStartBytes(const std::string startBytes){
  return this->receiveStartBytes((const unsigned char *) startBytes.c_str(), startBytes.length());
}

/**
 * @brief Performs a TCP/IP data receiving operation until the specified stop bytes are detected.
 *
 * This function receives TCP/IP data until the specified stop bytes are detected. Any TCP/IP data received up to and including the stop bytes is automatically stored in the buffer. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] stopBytes The data representing the stop bytes to be detected.
 * @param[in] sz The size of the stop bytes data to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveUntillStopBytes(const unsigned char *stopBytes, size_t sz){
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
 * This function receives TCP/IP data until the specified stop bytes are detected. Any TCP/IP data received up to and including the stop bytes is automatically stored in the buffer. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] stopBytes A vector of `unsigned char` representing the stop bytes to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveUntillStopBytes(const std::vector <unsigned char> stopBytes){
  return this->receiveUntillStopBytes(stopBytes.data(), stopBytes.size());
}

/**
 * @brief Overloaded function for `receiveUntillStopBytes` with input as `const char*`.
 *
 * This function receives TCP/IP data until the specified stop bytes are detected. Any TCP/IP data received up to and including the stop bytes is automatically stored in the buffer. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] stopBytes A pointer to a null-terminated character array representing the stop bytes to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveUntillStopBytes(const char *stopBytes){
  return this->receiveUntillStopBytes((const unsigned char *) stopBytes, strlen(stopBytes));
}

/**
 * @brief Overloaded function for `receiveUntillStopBytes` with input as `std::string`.
 *
 * This function receives TCP/IP data until the specified stop bytes are detected. Any TCP/IP data received up to and including the stop bytes is automatically stored in the buffer. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] stopBytes A string representing the stop bytes to be detected.
 * @return `0` if the operation is successful.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 */
int TCP::receiveUntillStopBytes(const std::string stopBytes){
  return this->receiveUntillStopBytes((const unsigned char *) stopBytes.c_str(), stopBytes.length());
}

/**
 * @brief Receive TCP/IP data and checks if the data matches the specified stop bytes.
 *
 * This method performs TCP/IP data reception while simultaneously checking if the data matches the desired stop bytes. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] stopBytes A pointer to the stop bytes data to be detected.
 * @param[in] sz The size of the stop bytes data to be detected.
 * @return `0` if the operation is successful and the data is valid.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 * @return `3` if data is recieved but does not match the specified stop bytes.
 */
int TCP::receiveStopBytes(const unsigned char *stopBytes, size_t sz){
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
 * This method performs TCP/IP data reception while simultaneously checking if the data matches the desired stop bytes. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] stopBytes A `std::vector` containing the stop bytes data to be detected.
 * @return `0` if the operation is successful and the data is valid.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 * @return `3` if data is received but does not match the specified stop bytes.
 */
int TCP::receiveStopBytes(const std::vector <unsigned char> stopBytes){
  return this->receiveStopBytes(stopBytes.data(), stopBytes.size());
}

/**
 * @brief Function overloading for `receiveStopBytes` with input using `char *`.
 *
 * This method performs TCP/IP data reception while simultaneously checking if the data matches the desired stop bytes. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] stopBytes A C-style string (null-terminated) representing the stop bytes to be detected.
 * @return `0` if the operation is successful and the data is valid.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 * @return `3` if data is received but does not match the specified stop bytes.
 */
int TCP::receiveStopBytes(const char *stopBytes){
  return this->receiveStopBytes((const unsigned char *) stopBytes, strlen(stopBytes));
}

/**
 * @brief Function overloading for `receiveStopBytes` with input using `std::string`.
 *
 * This method performs TCP/IP data reception while simultaneously checking if the data matches the desired stop bytes. The received TCP/IP data can be accessed using the `TCP::getBuffer` method.
 *
 * @param[in] stopBytes A `std::string` object representing the stop bytes to be detected.
 * @return `0` if the operation is successful and the data is valid.
 * @return `1` if the port is not open.
 * @return `2` if a timeout occurs.
 * @return `3` if data is received but does not match the specified stop bytes.
 */
int TCP::receiveStopBytes(const std::string stopBytes){
  return this->receiveStopBytes((const unsigned char *) stopBytes.c_str(), stopBytes.length());
}