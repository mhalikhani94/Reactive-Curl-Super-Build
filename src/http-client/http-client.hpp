#pragma once

#include "request-builder.hpp"
#include "../types.hpp"

#include <mutex>

class HttpClient
{
public:
    HttpClient(const bool init = true)
    {
        if (init)
        {
            static std::once_flag flag;
            std::call_once(flag, []
            {
                curl_global_init(CURL_GLOBAL_DEFAULT);
            });
        }
    }

    virtual ~HttpClient()
    {
    }

    std::unique_ptr<RequestBuilder> build()
    {
        return std::make_unique<RequestBuilder>();
    }
};