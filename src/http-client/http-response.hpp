#ifndef REACTIVE_CURL_HTTP_RESPONSE_HPP_
#define REACTIVE_CURL_HTTP_RESPONSE_HPP_

#include "../types.hpp"

#include <curl/curl.h>

#include <string>
#include <functional>

struct HttpResponse
{
    HttpResponse() = default;

    HttpResponse(const CURLcode& code)
    {
        curl_code = code;
        error_msg = curl_easy_strerror(code);
    }
    
    bool is_ok() const
    {
        if (curl_code == CURLE_OK)
        {
            if ((http_response_code >= 200) && (http_response_code < 300))
            {
                return true;
            }
        }
        return false;
    }

    CURLcode curl_code{};
    long http_response_code{};
    std::string error_msg{};
    std::string body{};
    std::string header{};
};

using completion_fn_t = std::function<void (const HttpResponse& result)>;

#endif //REACTIVE_CURL_HTTP_RESPONSE_HPP_