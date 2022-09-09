#include "http-request.hpp"

HttpRequest::HttpRequest()
: curl_handle{std::make_unique<CurlHandle>()}
{}

HttpRequest::HttpRequest(CurlHandle::ptr_t&& handle)
: curl_handle{std::move(handle)}
{}

HttpRequest::~HttpRequest()
{
    if (headers)
    {
        curl_slist_free_all(headers);
    }
}

void HttpRequest::prepare(const RequestType type, completion_fn_t fn)
{
    request_type = type;
    set_request_type();
    completion = std::move(fn);
}

void HttpRequest::execute()
{
    const auto result = curl_easy_perform(*curl_handle);
    call_completion(result);
}

CurlHandle& HttpRequest::get_curl_handle()
{
    assert(curl_handle); return *curl_handle;
}

RequestType HttpRequest::get_request_type()
{
    return request_type;
}

void HttpRequest::set_default_input_handler(std::unique_ptr<DataHandlerBase> ptr)
{
    default_in_handler = std::move(ptr);
}

void HttpRequest::set_default_output_handler(std::unique_ptr<DataHandlerBase> ptr)
{
    default_out_handler = std::move(ptr);
}

void HttpRequest::set_default_header_handler(std::unique_ptr<DataHandlerBase> ptr)
{
    default_header_handler = std::move(ptr);
}

HttpRequest::headers_t& HttpRequest::get_headers()
{
    return headers;
}

std::string& HttpRequest::get_default_input_buffer()
{
    return default_data_buffer;
}

std::string& HttpRequest::get_default_header_input_buffer()
{
    return default_header_buffer;
}

void HttpRequest::call_completion(CURLcode curl_code)
{
    HttpResponse result(curl_code);

    curl_easy_getinfo (*curl_handle, CURLINFO_RESPONSE_CODE,
                        &result.http_response_code);
    if (completion)
    {
        if (!default_data_buffer.empty())
        {
            result.body = std::move(default_data_buffer);
        }
        if (!default_header_buffer.empty())
        {
            result.header = std::move(default_header_buffer);
        }
        completion(result);
    }
}

void HttpRequest::set_request_type()
{
    switch(request_type)
    {
        case RequestType::kGet:
            curl_easy_setopt(*curl_handle, CURLOPT_HTTPGET, 1L);
            break;
        case RequestType::kPut:
            headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
            curl_easy_setopt(*curl_handle, CURLOPT_UPLOAD, 1L);
            break;
        case RequestType::kPost:
            headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
            curl_easy_setopt(*curl_handle, CURLOPT_UPLOAD, 0L);
            curl_easy_setopt(*curl_handle, CURLOPT_POST, 1L);
            break;
        case RequestType::kHead:
            curl_easy_setopt(*curl_handle, CURLOPT_NOBODY, 1L);
            break;
        case RequestType::kOptions:
            curl_easy_setopt(*curl_handle, CURLOPT_CUSTOMREQUEST, "OPTIONS");
            break;
        case RequestType::kPatch:
            headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
            curl_easy_setopt(*curl_handle, CURLOPT_CUSTOMREQUEST, "PATCH");
            break;
        case RequestType::kDelete:
            curl_easy_setopt(*curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        default:
            throw Exception("Unsupported request type" + std::to_string(static_cast<int>(request_type)));
    }
}