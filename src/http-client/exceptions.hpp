#ifndef REACTIVE_CURL_EXCEPTIONS_HPP_
#define REACTIVE_CURL_EXCEPTIONS_HPP_

#include "../types.hpp"

#include <curl/curl.h>

#include <stdexcept>
#include <string>

class Exception : public std::runtime_error
{
public:
    Exception(const std::string& msg) : runtime_error(msg) {}
};

class CurlException : public Exception
{
public:
    CurlException(const std::string msg, const CURLcode err)
        : Exception(msg + '(' + std::to_string(err) + "): " + curl_easy_strerror(err))
        , error_code{err}
        {}

    int get_error_code() const { return error_code; }

private:
    const int error_code;
};

#endif //REACTIVE_CURL_EXCEPTIONS_HPP_