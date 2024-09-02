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
  unsigned short cnt = 0;
  unsigned short value = 0;
  do {
    if (*address == '.'){
      value = 0;
      cnt++;
    }
    else if (*address >= 0x30 && *address <= 0x39){
      value *= 10;
      value += static_cast<unsigned short>(*address - 0x30);
      if (value >= 255){
        return false;
      }
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