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
#include <iostream>
#include "tcp-server.hpp"

struct thHandler_t {
  ClientCollection *currentCollection;
  TCPServer *server;
};

void *receptionThreadHandler(void *ptr){
  struct thHandler_t *strc = (struct thHandler_t *) ptr;
  ClientCollection *obj = strc->currentCollection;
  TCPServer *server = strc->server;
  pthread_mutex_lock(&(obj->mtx));
  pthread_cond_signal(&(obj->cond));
  pthread_mutex_unlock(&(obj->mtx));
  std::cout << __func__ << ": thread start" << std::endl;
  void (*callback)(SynapSock &, void *) = (void (*)(SynapSock &, void *))server->getReceptionHandlerFunction();
  callback(*(obj->client), server->getReceptionHandlerParam());
  pthread_mutex_lock(&(obj->mtx));
  pthread_detach(obj->th);
  obj->th = 0;
  gettimeofday(&(obj->lastActivity), nullptr);
  if (obj->pipe[1]) write(obj->pipe[1], "\x30", 1);
  pthread_mutex_unlock(&(obj->mtx));
  std::cout << __func__ << ": thread end" << std::endl;
  return nullptr;
}

ClientCollection::ClientCollection(const SynapSock *client){
  gettimeofday(&lastActivity, nullptr);
  this->th = 0;
  this->pipe[0] = 0;
  this->pipe[1] = 0;
  pthread_cond_init(&(this->cond), nullptr);
  pthread_mutex_init(&(this->mtx), nullptr);
  this->next = nullptr;
  this->client = (SynapSock *) client;
}

ClientCollection::~ClientCollection(){
  if (client) delete client;
  if (this->pipe[0] > 0){
    close(this->pipe[0]);
  }
  if (this->pipe[1] > 0){
    close(this->pipe[1]);
  }
  pthread_cond_destroy(&(this->cond));
  pthread_mutex_destroy(&(this->mtx));
  if (this->th > 0){
    pthread_join(this->th, nullptr);
    this->th = 0;
  }
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
TCPServer::TCPServer(const unsigned char *address) : SynapSock(address){
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
TCPServer::TCPServer(const unsigned char *address, int port) : SynapSock(address, port){
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
TCPServer::TCPServer(const std::vector <unsigned char> address) : SynapSock(address){
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
TCPServer::TCPServer(const std::vector <unsigned char> address, int port) : SynapSock(address, port){
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
TCPServer::TCPServer(const char *address) : SynapSock(address){
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
TCPServer::TCPServer(const char *address, int port) : SynapSock(address, port){
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
TCPServer::TCPServer(const std::string address) : SynapSock(address){
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
TCPServer::TCPServer(const std::string address, int port) : SynapSock(address, port){
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
  SynapSock *client = new SynapSock;
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
bool TCPServer::removeClient(const SynapSock *socket){
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
      if(cList->pipe[0] <= 0){
        FD_SET(cList->client->getSocketFd(), &readfds);
        max = max < cList->client->getSocketFd() ? cList->client->getSocketFd() : max;
      }
      else {
        FD_SET(cList->pipe[0], &readfds);
        max = max < cList->pipe[0] ? cList->pipe[0] : max;
      }
      cList = cList->next;
    } while (cList != this->clientList);
  }
  tv.tv_sec = timeoutMs / 1000;
  tv.tv_usec = (timeoutMs % 1000) * 1000;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
  ret = select(max + 1 , &readfds , NULL , NULL , &tv);
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  if(ret >= 0){
    if (FD_ISSET(this->sockFd, &readfds)){
      if (this->conReqCallbackFunction != nullptr){
        void (*callback)(TCPServer &, void *) = (void (*)(TCPServer &, void *))this->conReqCallbackFunction;
        void *param = this->conReqCallbackParam;
        pthread_mutex_unlock(&(this->mtx));
        pthread_mutex_unlock(&(this->wmtx));
        callback(*this, param);
      }
      else {
        pthread_mutex_unlock(&(this->mtx));
        pthread_mutex_unlock(&(this->wmtx));
        this->acceptNewClient();
      }
      return EVENT_CONNECT_REQUEST;
    }
    else if (this->clientList != nullptr) {
      cList = this->clientList;
      gettimeofday(&tv, NULL);
      do {
        if (cList->pipe[0] > 0){
          if (FD_ISSET(cList->pipe[0], &readfds)){
            close(cList->pipe[0]);
            close(cList->pipe[1]);
            cList->pipe[0] = 0;
            cList->pipe[1] = 0;
          }
        }
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
          if (this->receptionCallbackFunction != nullptr){
            if (this->receptionHandlerAsThread == false){
              void (*callback)(SynapSock &, void *) = (void (*)(SynapSock &, void *))this->receptionCallbackFunction;
              callback(*(this->client), this->conReqCallbackParam);
            }
            else {
              struct thHandler_t handlerPointer = {cList, this};
              if (pipe(cList->pipe) == -1){
                cList->pipe[0] = 0;
                cList->pipe[1] = 0;
              }
              if (pthread_create(&(cList->th), nullptr, receptionThreadHandler, (void *) &handlerPointer) == 0){
                pthread_mutex_lock(&(cList->mtx));
                pthread_cond_wait(&(cList->cond), &(cList->mtx));
                pthread_mutex_unlock(&(cList->mtx));
              }
            }
          }
          return EVENT_BYTES_AVAILABLE;
        }
        if (cList->pipe[0] <= 0){
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
SynapSock *TCPServer::getActiveClient(){
  return this->client;
}

/**
 * @brief Provides access to users to manage connection requests.
 *
 * This method provide flexibility to users to manage existing connection requests.
 * The callback function is automatically called when there is a connection request event.
 * If the callback function is not set up, the connection request will be automatically
 * approved (accepted) by the server.
 *
 * @param[in] func callback function that has 2 parameters. `TCP Server &` is an object of the server itself. `void *` is a pointer that will connect directly to `void *param`.
 * @param[in] param callback function parameter.
 */
void TCPServer::setConnectionRequestHandler(void (*func)(TCPServer &, void *), void *param){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->conReqCallbackFunction = (const void *) func;
  this->conReqCallbackParam = param;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
}

/**
 * @brief Set handler to receive data sent by remote client.
 *
 * This method must be called before socket communication begins (before calling the `eventCheck` method).
 *
 * @param[in] func callback function that has 2 parameters. `SynapSock &` is an active connection. `void *` is a pointer that will connect directly to `void *param`
 * @param[in] param callback function parameter.
 * @param[in] asThread if the given value is true, then the reception handler will run as a thread.
 */
void TCPServer::setReceptionHandler(void (*func)(SynapSock &, void *), void *param, bool asThread){
  pthread_mutex_lock(&(this->mtx));
  pthread_mutex_lock(&(this->wmtx));
  this->receptionCallbackFunction = (const void *) func;
  this->receptionCallbackParam = param;
  this->receptionHandlerAsThread = asThread;
  pthread_mutex_unlock(&(this->mtx));
  pthread_mutex_unlock(&(this->wmtx));
}

/**
 * @brief Method overloading setReceptionHandler. Set handler to receive data sent by remote client for non-thread operation.
 *
 * This method must be called before socket communication begins (before calling the `eventCheck` method).
 *
 * @param[in] func callback function that has 2 parameters. `SynapSock &` is an active connection. `void *` is a pointer that will connect directly to `void *param`
 * @param[in] param callback function parameter.
 */
void TCPServer::setReceptionHandler(void (*func)(SynapSock &, void *), void *param){
  this->setReceptionHandler(func, param, false);
}

/**
 * @brief Retrieves the pointer address for the data reception handler function.
 *
 * @return pointer of reception handler function.
 */
const void *TCPServer::getReceptionHandlerFunction(){
  return this->receptionCallbackFunction;
}

/**
 * @brief Retrieves the pointer address for the data reception handler parameter.
 *
 * @return pointer of reception handler parameter.
 */
void *TCPServer::getReceptionHandlerParam(){
  return this->receptionCallbackParam;
}