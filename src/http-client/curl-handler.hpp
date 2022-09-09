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

    CurlHandle() = default;

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