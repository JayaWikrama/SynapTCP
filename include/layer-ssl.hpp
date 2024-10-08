/*
 * $Id: layer-ssl.hpp,v 1.0.0 2024/10/08 22:00:00 Jaya Wikrama Exp $
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
 * @brief SSL layer for socket base communication.
 *
 * This library provides a SSL implementation for socket base communication.
 *
 * @version 1.0.0
 * @date 2024/10/08
 * @author Jaya Wikrama
 */

#ifndef __LAYER_SSL_HPP__
#define __LAYER_SSL_HPP__

#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>

class SSLWarper {
  private:
    SSL_CTX *ctx;
    void init();
    void cleanup();
    SSL_CTX *createServerContext();
    SSL_CTX *createClientContext();
    void loadCertificates();
  public:
    typedef enum _CONTEXT_TYPE_t {
      CTX_TYPE_SERVER = 0,
      CTX_TYPE_CLIENT
    } CONTEXT_TYPE_t;
    SSLWarper(_CONTEXT_TYPE_t type, const std::string cert, const std::string key);
    ~SSLWarper();
    SSL *createSSL(int sockFd);
    bool acceptSSL(SSL *ssl);
    bool connectSSL(SSL *ssl);
    SSL_CTX *getContext();
};

#endif
