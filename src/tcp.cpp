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
  if (this->isUseSSL == false || isUseSSL == false){
    this->sslVerifyMode = false;
  }
  this->isUseSSL = isUseSSL;
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
  if (this->isUseSSL == false){
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

