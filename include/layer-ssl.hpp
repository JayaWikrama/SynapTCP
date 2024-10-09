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
 * @brief A wrapper class that simplifies SSL initialization, certificate/key loading, and SSL connection handling.
 *
 * This class provides an easy way to create SSL contexts for both server and client modes. It allows
 * certificate and key loading directly from strings and offers methods to handle SSL connections.
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
    SSL_CTX *ctx;                   /*!< The SSL context used for handling SSL/TLS operations. */
    SSL *ssl;                       /*!< SSL descriptor. */

    /**
     * @brief Initializes the OpenSSL library.
     *
     * This method loads the necessary OpenSSL error strings and algorithms.
     */
    void init();

    /**
     * @brief Cleans up the OpenSSL library.
     *
     * This method frees up resources allocated by OpenSSL.
     */
    void cleanup();

    /**
     * @brief Creates an SSL context for the server.
     *
     * This method sets up the SSL context for server mode and returns the created context.
     *
     * @return A pointer to the created SSL_CTX object.
     */
    SSL_CTX *createServerContext();

    /**
     * @brief Creates an SSL context for the client.
     *
     * This method sets up the SSL context for client mode and returns the created context.
     *
     * @return A pointer to the created SSL_CTX object.
     */
    SSL_CTX *createClientContext();

    /**
     * @brief Loads the certificate and private key into the SSL context.
     *
     * This method loads the certificate and private key from the provided PEM strings and configures the SSL context
     * for use in server mode.
     *
     * @param cert A string containing the server's certificate in PEM format.
     * @param key A string containing the server's private key in PEM format.
     */
    void loadCertificates(const std::string cert, const std::string key);

  public:
    typedef enum _CONTEXT_TYPE_t {
      CTX_TYPE_SERVER = 0,
      CTX_TYPE_CLIENT
    } CONTEXT_TYPE_t;

    /**
     * @brief Constructs an SSLWrapper instance and initializes the SSL context.
     *
     * The constructor will initialize OpenSSL, create an SSL context depending on whether this instance is for a server
     * or client, and optionally load the certificate and private key from the provided PEM strings.
     *
     * @param type A CONTEXT_TYPE Enumeration that indicating whether this instance is for a server (true) or a client (false).
     * @param cert Optional string containing the server's certificate in PEM format. Only used if `is_server` is true.
     * @param key Optional string containing the server's private key in PEM format. Only used if `is_server` is true.
     */
    SSLWarper(SSLWarper::CONTEXT_TYPE_t type, const std::string cert, const std::string key);

    /**
     * @brief Destructor for SSLWrapper.
     *
     * Cleans up the SSL context and shuts down OpenSSL when the object is destroyed.
     */
    ~SSLWarper();

    /**
     * @brief Creates an SSL descriptor associated with a socket file descriptor.
     *
     * This method creates a new SSL object that will be used to handle the SSL session for the given socket file descriptor.
     *
     * @param sockFd The file descriptor for the TCP connection socket.
     * @return True if success to create SSL descriptor.
     */
    bool createSSL(int sockFd);

    /**
     * @brief Accepts an SSL connection on the server side.
     *
     * This method performs the SSL/TLS handshake on the server side, accepting an incoming SSL connection.
     *
     * @param ssl The SSL object created using `create_ssl`.
     * @return True if the SSL handshake was successful, false otherwise.
     */
    bool acceptSSL(SSL *ssl);

    /**
     * @brief Initiates an SSL connection on the client side.
     *
     * This method performs the SSL/TLS handshake on the client side, initiating an SSL connection to a server.
     *
     * @param ssl The SSL object created using `create_ssl`.
     * @return True if the SSL handshake was successful, false otherwise.
     */
    bool connectSSL(SSL *ssl);

    /**
     * @brief Returns the SSL context used by this instance.
     *
     * This method provides access to the underlying SSL context used by the `SSLWrapper` instance. It can be useful
     * if further configuration of the context is needed.
     *
     * @return A pointer to the SSL_CTX object representing the SSL context.
     */
    SSL_CTX *getContext();

    /**
     * @brief Returns the SSL descriptor used by this instance.
     *
     * @return A pointer to the SSL descriptor representing SSL connection.
     *
     */
    SSL *getSSLDescriptor();
};

#endif
