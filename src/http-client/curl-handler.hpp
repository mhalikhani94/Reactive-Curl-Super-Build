#ifndef REACTIVE_CURL_CURL_HANDLER_HPP_
#define REACTIVE_CURL_CURL_HANDLER_HPP_

#include "../types.hpp"

#include <curl/curl.h>

#include <memory>

class CurlHandle
{
public:
    using ptr_t = std::unique_ptr<CurlHandle>;
    using curl_handle_t = decltype(curl_easy_init());

    CurlHandle()
    {
        curl_easy_setopt(handle, CURLOPT_SSLCERTTYPE, "PEM");
        curl_easy_setopt(handle, CURLOPT_CAINFO, "../src/http-client/cacert.pem");
    }

    ~CurlHandle()
    {
        if (handle)
        {
            curl_easy_cleanup(handle);
            handle = nullptr;
        }
    }

    operator curl_handle_t () const { return handle; } //TODO: remove

private:
    curl_handle_t handle{curl_easy_init()};
};

#endif //REACTIVE_CURL_CURL_HANDLER_HPP_