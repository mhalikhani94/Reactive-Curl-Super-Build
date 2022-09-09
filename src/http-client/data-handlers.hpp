#ifndef REACTIVE_CURL_DATA_HANDLERS_HPP_
#define REACTIVE_CURL_DATA_HANDLERS_HPP_

#include "../types.hpp"

#include <iterator>
#include <string>

struct DataHandlerBase
{
    virtual ~DataHandlerBase() = default;
};

template <typename T>
struct InputDataHandler : public DataHandlerBase
{
    InputDataHandler(T& data_) : data{data_}
    {}

    static size_t write_callback(char *ptr, size_t size, size_t nitems, void *userdata)
    {
        assert(userdata);
        auto self = reinterpret_cast<InputDataHandler *>(userdata);
        const auto bytes = size * nitems;
        if (bytes > 0)
        {
            std::copy(ptr, ptr + bytes, std::back_inserter(self->data));
        }
        return bytes;
    }

    T& data;
};

template <typename T>
struct InputHeaderHandler : public DataHandlerBase
{
    InputHeaderHandler(T& data_) : data{data_}
    {}

    static size_t write_callback(char *ptr, size_t size, size_t nitems, void *userdata)
    {
        assert(userdata);
        auto self = reinterpret_cast<InputHeaderHandler *>(userdata);
        const auto bytes = size * nitems;
        if (bytes > 0)
        {
            std::copy(ptr, ptr + bytes, std::back_inserter(self->data));
            ptr[bytes-1] = 0;
        }
        return bytes;
    }

    T& data;
};

template <typename T>
struct OutputDataHandler : public DataHandlerBase
{
    OutputDataHandler() = default;
    OutputDataHandler(const T& v) : data{v} {}
    OutputDataHandler(T&& v) : data{std::move(v)} {}

    static size_t read_callback(char *bufptr, size_t size, size_t nitems, void *userdata)
    {
        assert(userdata);
        OutputDataHandler *self = reinterpret_cast<OutputDataHandler *>(userdata);
        const auto bytes = size * nitems;
        auto out_bytes = std::min<size_t>(bytes, (self->data.size() - self->sendt_bytes));
        std::copy(self->data.cbegin() + self->sendt_bytes, self->data.cbegin() + (self->sendt_bytes + out_bytes),bufptr);
        self->sendt_bytes += out_bytes;
        return out_bytes;
    }

    T data;
    size_t sendt_bytes = 0;
};

#endif //REACTIVE_CURL_DATA_HANDLERS_HPP_