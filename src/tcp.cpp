#include <stdlib.h>
#include <string.h>
#include "tcp.hpp"

static void __TCP(TCP *obj){
  obj->maxClient = 10;
  obj->port = 3000;
  obj->address.clear();
  obj->sockFd = -1;
  obj->connFd = -1;
  obj->keepAliveMs = 0;
  memset(&(obj->len), 0x00, sizeof(obj->len));
  obj->timeout.tv_sec = 1;
  obj->timeout.tv_usec = 500000;
  struct sockaddr_in addr;
  memset(&(obj->addr), 0x00, sizeof(obj->addr));
#ifdef __STCP_SSL__
  obj->useSSL = false;
  obj->sslVerifyMode = false;
  obj->sslConnRoutineOkStatus = 0;
  obj->sslConn = nullptr;
  obj->sslCtx = nullptr;
#endif
  pthread_mutex_init(&(obj->mtx), nullptr);
  pthread_mutex_init(&(obj->wmtx), nullptr);
  obj->data.clear();
  obj->remainingData.clear();
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
  __TCP(this);
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
  __TCP(this);
  this->setAddress(address);
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
TCP(const unsigned char *address, int port){
  __TCP(this);
  this->setAddress(address);
  this->setPort(port);
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
TCP(const std::vector <unsigned char> address){
  __TCP(this);
  this->setAddress(address);
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
TCP(const std::vector <unsigned char> address, int port){
  __TCP(this);
  this->setAddress(address);
  this->setPort(port);
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
TCP(const char *address){
  __TCP(this);
  this->setAddress(address);
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
TCP(const char *address, int port){
  __TCP(this);
  this->setAddress(address);
  this->setPort(port);
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
TCP(const std::string address){
  __TCP(this);
  this->setAddress(address);
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
TCP(const std::string address, int port){
  __TCP(this);
  this->setAddress(address);
  this->setPort(port);
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
  if (address.size != 4) return false;
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
  this->address.assign(address, address + 4);
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
  this->address.assign(address.begin(), address.end());
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
  return this->setAddress(address.c_str())
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
  this->port = port;
  return true;
}