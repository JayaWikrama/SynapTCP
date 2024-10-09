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

#include <iostream>
#include "layer-ssl.hpp"

/**
 * @brief Initializes the OpenSSL library.
 *
 * This method loads the necessary OpenSSL error strings and algorithms.
 */
void SSLWarper::init(){
    this->ctx = nullptr;
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

/**
 * @brief Cleans up the OpenSSL library.
 *
 * This method frees up resources allocated by OpenSSL.
 */
void SSLWarper::cleanup(){
    EVP_cleanup();
}

/**
 * @brief Creates an SSL context for the server.
 *
 * This method sets up the SSL context for server mode and returns the created context.
 *
 * @return A pointer to the created SSL_CTX object.
 */
SSL_CTX *SSLWarper::createServerContext(){
    const SSL_METHOD* method = SSLv23_server_method();
    SSL_CTX* result = SSL_CTX_new(method);
    if (!result) {
        std::cerr << "Unable to create SSL context" << std::endl;
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * @brief Creates an SSL context for the client.
 *
 * This method sets up the SSL context for client mode and returns the created context.
 *
 * @return A pointer to the created SSL_CTX object.
 */
SSL_CTX *SSLWarper::createClientContext(){
    const SSL_METHOD* method = SSLv23_client_method();
    SSL_CTX* result = SSL_CTX_new(method);
    if (!result) {
        std::cerr << "Unable to create SSL context" << std::endl;
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * @brief Loads the certificate and private key into the SSL context.
 *
 * This method loads the certificate and private key from the provided PEM strings and configures the SSL context
 * for use in server mode.
 *
 * @param cert A string containing the server's certificate in PEM format.
 * @param key A string containing the server's private key in PEM format.
 */
void SSLWarper::loadCertificates(const std::string cert, const std::string key){
    BIO* bio_cert = BIO_new_mem_buf((void*) cert.c_str(), -1);
    X509* certX = PEM_read_bio_X509(bio_cert, nullptr, 0, nullptr);
    if (!certX || SSL_CTX_use_certificate(this->ctx, certX) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    BIO_free(bio_cert);
    X509_free(certX);
    BIO* bio_key = BIO_new_mem_buf((void*) key.c_str(), -1);
    EVP_PKEY* keyE = PEM_read_bio_PrivateKey(bio_key, nullptr, 0, nullptr);
    if (!keyE || SSL_CTX_use_PrivateKey(this->ctx, keyE) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    BIO_free(bio_key);
    EVP_PKEY_free(keyE);
}

/**
 * @brief Constructs an SSLWrapper instance and initializes the SSL context.
 *
 * The constructor will initialize OpenSSL, create an SSL context depending on whether this instance is for a server
 * or client, and optionally load the certificate and private key from the provided PEM strings.
 *
 * @param type A CONTEXT_TYPE Enumeration that indicating whether this instance is for a server or a client.
 * @param cert Optional string containing the server's certificate in PEM format. Only used for server.
 * @param key Optional string containing the server's private key in PEM format. Only used for server.
 */
SSLWarper::SSLWarper(SSLWarper::CONTEXT_TYPE_t type, const std::string cert, const std::string key){
    this->init();
    if (type == CTX_TYPE_SERVER){
        this->ctx = this->createServerContext();
        if (cert.length() > 0 && key.length() > 0){
            this->loadCertificates(cert, key);
        }
    }
    else {
        this->ctx = this->createClientContext();
    }
}

/**
 * @brief Destructor for SSLWrapper.
 *
 * Cleans up the SSL context and shuts down OpenSSL when the object is destroyed.
 */
SSLWarper::~SSLWarper(){
    if (this->ctx) SSL_CTX_free(this->ctx);
    this->cleanup();
}

/**
 * @brief Creates an SSL descriptor associated with a socket file descriptor.
 *
 * This method creates a new SSL object that will be used to handle the SSL session for the given socket file descriptor.
 *
 * @param sockFd The file descriptor for the TCP connection socket.
 * @return True if success to create SSL descriptor.
 */
SSL *SSLWarper::createSSL(int sockFd){
    SSL *ssl = SSL_new(ctx);
    if (ssl == nullptr) return nullptr;
    if (SSL_set_fd(ssl, sockFd) != 1){
        SSL_free(ssl);
        return nullptr;
    }
    return ssl;
}

/**
 * @brief Accepts an SSL connection on the server side.
 *
 * This method performs the SSL/TLS handshake on the server side, accepting an incoming SSL connection.
 *
 * @param ssl SSL descriptor.
 * @return True if the SSL handshake was successful, false otherwise.
 */
bool SSLWarper::acceptSSL(SSL *ssl){
    return SSL_accept(ssl) > 0;
}

/**
 * @brief Initiates an SSL connection on the client side.
 *
 * This method performs the SSL/TLS handshake on the client side, initiating an SSL connection to a server.
 *
 * @param ssl SSL descriptor.
 * @return True if the SSL handshake was successful, false otherwise.
 */
bool SSLWarper::connectSSL(SSL *ssl){
    return SSL_connect(ssl) > 0;
}

/**
 * @brief Returns the SSL context used by this instance.
 *
 * This method provides access to the underlying SSL context used by the `SSLWrapper` instance. It can be useful
 * if further configuration of the context is needed.
 *
 * @return A pointer to the SSL_CTX object representing the SSL context.
 */
SSL_CTX *SSLWarper::getContext(){
    return this->ctx;
}

/**
 * @brief Release SSL descryptor pointer.
 *
 */
void SSLWarper::deleteSSLDescryptor(SSL *ssl){
    SSL_free(ssl);
}
