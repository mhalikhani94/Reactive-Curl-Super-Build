#ifndef REACTIVE_CURL_HTTP_REQUEST_HPP_
#define REACTIVE_CURL_HTTP_REQUEST_HPP_

#include "exceptions.hpp"
#include "curl-handler.hpp"
#include "http-response.hpp"
#include "options.hpp"
#include "data-handlers.hpp"
#include "../types.hpp"

class HttpRequest
{
public:
    using ptr_t = std::unique_ptr<HttpRequest>;
    using headers_t = curl_slist*;

    HttpRequest();

    HttpRequest(CurlHandle::ptr_t&& handle);

    ~HttpRequest();

    void prepare(const RequestType type, completion_fn_t fn);

    void execute();

    void set_default_input_handler(std::unique_ptr<DataHandlerBase> ptr);

    void set_default_output_handler(std::unique_ptr<DataHandlerBase> ptr);

    void set_default_header_handler(std::unique_ptr<DataHandlerBase> ptr);
    
    headers_t& get_headers();

    std::string& get_default_input_buffer();

    std::string& get_default_header_input_buffer();

    CurlHandle& get_curl_handle();
    
    RequestType get_request_type();

private:
    void call_completion(CURLcode curl_code);

    void set_request_type();

    CurlHandle::ptr_t curl_handle;
    RequestType request_type = RequestType::kInvalid;
    completion_fn_t completion;
    std::unique_ptr<DataHandlerBase> default_out_handler;
    std::unique_ptr<DataHandlerBase> default_in_handler;
    std::unique_ptr<DataHandlerBase> default_header_handler;
    headers_t headers = nullptr;
    std::string default_data_buffer;
    std::string default_header_buffer;
};

#endif //REACTIVE_CURL_HTTP_REQUEST_HPP_