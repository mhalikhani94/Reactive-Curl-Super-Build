#ifndef REACTIVE_CURL_REQUEST_BUILDER_HPP_
#define REACTIVE_CURL_REQUEST_BUILDER_HPP_

#include "http-request.hpp"
#include "../types.hpp"

class RequestBuilder
{
    static size_t write_callback(char *ptr, size_t size, size_t nitems, void *userdata)
    {
        const auto bytes = size * nitems;
        return bytes;
    }

public:
    using ptr_t = std::unique_ptr<RequestBuilder>;

    RequestBuilder();

    ~RequestBuilder() = default;

protected:
    RequestBuilder& prepare(RequestType rt, const std::string& url_);

public:
    RequestBuilder& Get(const std::string& url);

    RequestBuilder& Head(const std::string& url);

    RequestBuilder& Post(const std::string& url);

    RequestBuilder& Put(const std::string& url);

    RequestBuilder& Patch(const std::string& url);

    RequestBuilder& Delete(const std::string& url);

    RequestBuilder& Options(const std::string& url);

    RequestBuilder& add_header(const char *value);

    RequestBuilder& add_header(const std::string& name, const std::string& value);
    
    RequestBuilder& add_json_body(std::string body);

    RequestBuilder& accept_json();

    RequestBuilder& set_request_time_out(const long timeout);

    RequestBuilder& set_connect_timeout(const long timeout);

    RequestBuilder& process_response(completion_fn_t fn);

    RequestBuilder& set_custom_read_handler(size_t (*handler)(char *, size_t , size_t , void *), void *userdata);

    RequestBuilder& set_custom_write_handler(size_t (*handler)(char *, size_t , size_t , void *), void *userdata);

    void build();

    void send_request();

    template <typename T>
    RequestBuilder& add_curl_option(const CURLoption& opt, const T& value)
    {
        assert(!is_built);
        options->set_option(opt, value);
        return *this;
    }

    template <typename T>
    RequestBuilder& add_body(OutputDataHandler<T>& dh)
    {
        assert(!is_built);
        options->set_option(CURLOPT_READFUNCTION, dh.read_callback);
        options->set_option(CURLOPT_READDATA, &dh);
        have_data_out = true;
        return *this;
    }

    template <typename T>
    RequestBuilder& add_body(T data)
    {
        assert(!is_built);
        auto handler = std::make_unique<OutputDataHandler<T>>(std::move(data));
        auto& handler_ref = *handler;
        request->set_default_output_handler(std::move(handler));
        return add_body(handler_ref);
    }

    template <typename T>
    RequestBuilder& store_response(InputDataHandler<T>& dh)
    {
        assert(!is_built);
        options->set_option(CURLOPT_WRITEFUNCTION, dh.write_callback);
        options->set_option(CURLOPT_WRITEDATA, &dh);
        have_data_in = true;
        return *this;
    }

    template <typename T>
    RequestBuilder& store_response(T& data)
    {
        assert(!is_built);
        auto handler = std::make_unique<InputDataHandler<T>>(data);
        auto& handler_ref = *handler;
        request->set_default_input_handler(std::move(handler));
        return store_response(handler_ref);
    }

    template <typename T>
    RequestBuilder& store_header(InputHeaderHandler<T>& dh)
    {
        assert(!is_built);
        options->set_option(CURLOPT_HEADERFUNCTION, dh.write_callback);
        options->set_option(CURLOPT_HEADERDATA, &dh);
        have_header_in = true;
        return *this;
    }

    template <typename T>
    RequestBuilder& store_header(T& data)
    {
        assert(!is_built);
        auto handler = std::make_unique<InputHeaderHandler<T>>(data);
        auto& handler_ref = *handler;
        request->set_default_header_handler(std::move(handler));
        return store_header(handler_ref);
    }

private:
    std::unique_ptr<HttpRequest> request;
    std::unique_ptr<class Options> options;
    std::string url;
    RequestType request_type = RequestType::kInvalid;
    bool have_data_in = false;
    bool have_header_in = false;
    bool have_data_out = false;
    bool is_built = false;
    completion_fn_t completion;
    long request_timeout = 10000L; // 10 sec
    long connect_timeout = 3000L; // 1 sec
};

#endif //REACTIVE_CURL_REQUEST_BUILDER_HPP_