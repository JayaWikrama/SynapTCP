/*
 * $Id: tcp-server.cpp,v 1.0.1 2024/09/11 23:51:47 Jaya Wikrama Exp $
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
#include "tcp-server.hpp"

ClientCollection::ClientCollection(const Socket *client){
  gettimeofday(&lastActivity, nullptr);
  this->th = 0;
  pthread_cond_init(&(this->cond), nullptr);
  pthread_mutex_init(&(this->mtx), nullptr);
  this->next = nullptr;
  this->client = (Socket *) client;
}

ClientCollection::~ClientCollection(){
  if (client) delete client;
  pthread_cond_destroy(&(this->cond));
  pthread_mutex_destroy(&(this->mtx));
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
TCPServer::TCPServer(){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
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
TCPServer::TCPServer(const unsigned char *address) : Socket::Socket(address){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
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
 * @param[in] port The port of TCPServer/IP interface.
 */
TCPServer::TCPServer(const unsigned char *address, int port) : Socket::Socket(address, port){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
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
TCPServer::TCPServer(const std::vector <unsigned char> address) : Socket::Socket(address){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
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
 * @param[in] port The port of TCPServer/IP interface.
 */
TCPServer::TCPServer(const std::vector <unsigned char> address, int port) : Socket::Socket(address, port){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
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
TCPServer::TCPServer(const char *address) : Socket::Socket(address){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
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
 * @param[in] port The port of TCPServer/IP interface.
 */
TCPServer::TCPServer(const char *address, int port) : Socket::Socket(address, port){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
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
TCPServer::TCPServer(const std::string address) : Socket::Socket(address){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
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
 * @param[in] port The port of TCPServer/IP interface.
 */
TCPServer::TCPServer(const std::string address, int port) : Socket::Socket(address, port){
  this->maxClient = 10;
  this->client = nullptr;
  this->clientList = nullptr;
}

/**
 * @brief Destructor.
 *
 * This destructor is responsible for releasing any memory that has been allocated during the object's lifetime.
 * It ensures that all allocated resources are properly freed, preventing memory leaks.
 */
TCPServer::~TCPServer(){
  if (this->clientList != nullptr){
    ClientCollection *tmp =  this->clientList;
    ClientCollection *next = nullptr;
    do {
      next = this->clientList->next;
      delete tmp;
      tmp = next;
    } while (tmp != this->clientList);
  }
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
bool TCPServer::setMaximumClient(int nClient){
  if (nClient < 1 || nClient > 65535) return false;
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->maxClient = nClient;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Gets the maximum clients that the TCP/IP Server can handle.
 *
 * This getter function retrieves the maximum clients (the TCP/IP Server can handle) that currently configured for TCP/IP communication interface.
 *
 * @return The number of client that the TCP/IP Server can handle.
 */
int TCPServer::getMaximumClient(){
  return this->maxClient;
}

/**
 * @brief Initialize TCPServer/IP Server connection.
 *
 * This function attempts to open the specified TCPServer/IP (socket) port for communication. It initialize the port according to the current settings and prepares it for data transfer.
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
int TCPServer::init(){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  int retval = 0;
  const int optVal = 1;
  this->sockFd  = socket(AF_INET, SOCK_STREAM, 0);
  if (this->sockFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return 1;
  }
  this->addr.sin_family = AF_INET;
  memcpy(&(this->addr.sin_addr.s_addr), this->address.data(), 4);
  setsockopt(this->sockFd, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, sizeof(optVal));
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
 * @brief Add new client that has been accepted/connected by server to the client list.
 *
 * This method is called automatically when the `acceptNewClient` method is called sucessfully.
 * @param[in] connFd File descriptor for accepted client connection.
 * @return `true` on success
 * @return `false` if failed
 */
bool TCPServer::addClient(int connFd){
  char cliAddr[16];
  memset(cliAddr, 0x00, sizeof(cliAddr));
  inet_ntop(AF_INET, &(this->addr.sin_addr), cliAddr, INET_ADDRSTRLEN);
  Socket *client = new Socket;
  if (client == nullptr) return false;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  if (this->duplicate(*client) == false){
    pthread_mutex_lock(&(this->mtx));
    pthread_mutex_lock(&(this->wmtx));
    delete client;
    return false;
  }
  if (client->setAddress(cliAddr) == false){
    delete client;
    return false;
  }
  if (client->setPort(ntohs(this->addr.sin_port)) == false){
    delete client;
    return false;
  }
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  client->setSocketFd(connFd);
#ifdef __STCP_SSL__
  client->sslConn = this->sslConn;
  client->sslCtx = nullptr;
  this->sslConn = nullptr;
#endif
  ClientCollection *newClient = new ClientCollection(client);
  if (newClient == nullptr){
    delete client;
    return false;
  }
  if (this->clientList == nullptr){
    this->clientList = newClient;
    newClient->next = this->clientList;
  }
  else {
    ClientCollection *tmp = this->clientList;
    while (tmp->next != this->clientList){
      tmp = tmp->next;
    }
    tmp->next = newClient;
    newClient->next = this->clientList;
  }
  return true;
}

/**
 * @brief Remove existing client that has been disconnected by server from the client list.
 *
 * This method is called automatically when the `eventCheck` method is called sucessfully and disconnected event available.
 * @return `true` on success
 * @return `false` if failed
 */
bool TCPServer::removeClient(const Socket *socket){
  if (this->clientList == nullptr || socket == nullptr){
    return true;
  }
  if (this->clientList->client == socket && this->clientList == this->clientList->next){
    delete this->clientList;
    this->clientList = nullptr;
    if (this->client == socket) this->client = nullptr;
    return true;
  }
  ClientCollection *tmp = this->clientList;
  ClientCollection *prev = nullptr;
  if (this->clientList->client == socket){
    while (tmp->next != this->clientList){
      tmp = tmp->next;
    }
    prev = this->clientList;
    this->clientList = this->clientList->next;
    delete prev;
    if (this->client == socket) this->client = nullptr;
    return true;
  }
  do {
    prev = tmp;
    tmp = tmp->next;
    if (tmp->client == socket){
      prev->next = tmp->next;
      delete tmp;
      if (this->client == socket) this->client = nullptr;
      return true;
    }
  } while (tmp != this->clientList);
  return false;
}

/**
 * @brief Check available event on server side after server has been initialized.
 *
 * This function attempts to check available event on server side after server has been initialized.
 *
 * @param[in] timeoutMs maximum waiting time to check event.
 * @return `EVENT_NONE` when nothing happens
 * @return `EVENT_CONNECT_REQUEST` when there is a connection request from the client side (when receiving this event, the server side needs to call the `acceptNewClient` method)
 * @return `EVENT_BYTES_AVAILABLE` When there is data sent from the client side (to obtain this data, the server needs to call the reception method)
 * @return `EVENT_CLIENT_DISCONNECTED` when a client disconnects from the server
 */
TCPServer::SERVER_EVENT_t TCPServer::eventCheck(unsigned short timeoutMs){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  if (this->sockFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return EVENT_NONE;
  }
  fd_set readfds;
  unsigned short max = 0;
  int ret = 0;
  struct timeval tv;
  ClientCollection *cList = this->clientList;
  ClientCollection *tmp = nullptr;
  max = this->sockFd;
  FD_ZERO(&readfds);
  FD_SET(this->sockFd, &readfds);
  if (cList != nullptr){
    do {
      if(cList->client->getSocketFd() > 0){
        FD_SET(cList->client->getSocketFd(), &readfds);
        max = max < cList->client->getSocketFd() ? cList->client->getSocketFd() : max;
      }
      cList = cList->next;
    } while (cList != this->clientList);
  }
  tv.tv_sec = timeoutMs / 1000;
  tv.tv_usec = (timeoutMs % 1000) * 1000;
  ret = select(max + 1 , &readfds , NULL , NULL , &tv);
  if(ret >= 0){
    if (FD_ISSET(this->sockFd, &readfds)){
      pthread_mutex_unlock(&(this->mtx));
      pthread_mutex_unlock(&(this->wmtx));
      return EVENT_CONNECT_REQUEST;
    }
    else if (this->clientList != nullptr) {
      cList = this->clientList;
      gettimeofday(&tv, NULL);
      do {
        if (this->client == cList->client){
          tmp = cList;
        }
        else if (FD_ISSET(cList->client->getSocketFd(), &readfds)){
clientEvent:
          gettimeofday(&(cList->lastActivity), nullptr);
          this->client = cList->client;
          pthread_mutex_unlock(&(this->mtx));
          pthread_mutex_unlock(&(this->wmtx));
          if (this->client->isInputBytesAvailable() == false){
            pthread_mutex_lock(&(this->mtx));
            pthread_mutex_lock(&(this->wmtx));
            this->removeClient(this->client);
            pthread_mutex_unlock(&(this->mtx));
            pthread_mutex_unlock(&(this->wmtx));
            return EVENT_CLIENT_DISCONNECTED;
          }
          return EVENT_BYTES_AVAILABLE;
        }
        if (cList->client->isSocketTimeout(&tv, &(cList->lastActivity))) {
          if (cList->next != cList){
            tmp = cList->next;
          }
          else {
            tmp = nullptr;
          }
          this->removeClient(cList->client);
          if (tmp != nullptr) this->client = tmp->client;
          pthread_mutex_unlock(&(this->mtx));
          pthread_mutex_unlock(&(this->wmtx));
          return EVENT_CLIENT_DISCONNECTED;
        }
        cList = cList->next;
      } while(cList != this->clientList);
      if (tmp != nullptr){
        if (FD_ISSET(tmp->client->getSocketFd(), &readfds)){
          cList = tmp;
          goto clientEvent;
        }
      }
    }
  }
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return EVENT_NONE;
}

/**
 * @brief Accept the available client when TCPServer/IP Server listen the connection.
 *
 * This function attempts to accept the connection that requested by client side.
 *
 * @return `true` in success.
 * @return `false` if failed.
 */
bool TCPServer::acceptNewClient(){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  int connFd = 0;
  this->len = (socklen_t) sizeof(this->addr);
  connFd = accept(this->sockFd, (struct sockaddr *) &(this->addr), &(this->len));
  if (connFd <= 0){
    pthread_mutex_unlock(&(this->mtx));
    pthread_mutex_unlock(&(this->wmtx));
    return false;
  }
  if (this->tvTimeout.tv_sec > 0 ||  this->tvTimeout.tv_usec){
    struct timeval tv;
    tv.tv_sec = this->tvTimeout.tv_sec;
    tv.tv_usec = this->tvTimeout.tv_usec;
    setsockopt(connFd, SOL_SOCKET, SO_RCVTIMEO, (const unsigned char *) &tv, sizeof(tv));
  }
  this->addClient(connFd);
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  return true;
}

/**
 * @brief Gets the pointer of the currently active client connection.
 *
 * This method is responsible for returning the pointer value of an active client connection.
 *
 * @return pointer of active client object.
 * @return `nullptr` if no active client available.
 */
Socket *TCPServer::getActiveClient(){
  return this->client;
}