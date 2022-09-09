#ifndef REACTIVE_CURL_OPTIONS_HPP_
#define REACTIVE_CURL_OPTIONS_HPP_

#include "curl-handler.hpp"
#include "exceptions.hpp"
#include "../types.hpp"

#include <string>

class Options
{
public:
    Options(CurlHandle& handle) : curl_handle{handle} {}

    template <typename T>
    Options& set_option(const CURLoption& opt, const T& value)
    {
        const auto ret = curl_easy_setopt(curl_handle, opt, value);
        if (ret)
        {
            throw CurlException(std::string("Setting option ") + std::to_string(opt), ret);
        }
        return *this;
    }

    Options& set_option(const CURLoption& opt, const std::string& value)
    {
        return set_option(opt, value.c_str());
    }

private:
    CurlHandle& curl_handle;
};

#endif //REACTIVE_CURL_OPTIONS_HPP_