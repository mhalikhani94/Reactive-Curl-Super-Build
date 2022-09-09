#include "request-builder.hpp"

RequestBuilder::RequestBuilder()
: request{std::make_unique<HttpRequest>()}
, options{std::make_unique<class Options>(request->get_curl_handle())}
{

}

RequestBuilder& RequestBuilder::prepare(RequestType rt, const std::string& url_)
{
    assert(request_type == RequestType::kInvalid);
    assert(!is_built);
    request_type  = rt;
    url = url_;
    return *this;
}

RequestBuilder& RequestBuilder::Get(const std::string& url)
{
    return prepare(RequestType::kGet, url);
}

RequestBuilder& RequestBuilder::Head(const std::string& url)
{
    return prepare(RequestType::kHead, url);
}

RequestBuilder& RequestBuilder::Post(const std::string& url)
{
    return prepare(RequestType::kPost, url);
}

RequestBuilder& RequestBuilder::Put(const std::string& url)
{
    return prepare(RequestType::kPut, url);
}

RequestBuilder& RequestBuilder::Patch(const std::string& url)
{
    return prepare(RequestType::kPatch, url);
}

RequestBuilder& RequestBuilder::Delete(const std::string& url)
{
    return prepare(RequestType::kDelete, url);
}

RequestBuilder& RequestBuilder::Options(const std::string& url)
{
    return prepare(RequestType::kOptions, url);
}

RequestBuilder& RequestBuilder::add_header(const char *value)
{
    assert(value);
    assert(!is_built);
    request->get_headers() = curl_slist_append(request->get_headers(), value);
    return *this;
}

RequestBuilder& RequestBuilder::add_header(const std::string& name, const std::string& value)
{
    const auto v = name + ": " + value;
    return add_header(v.c_str());
}

RequestBuilder& RequestBuilder::add_json_body(std::string body)
{
    add_header("Content-type: Application/json; charset=utf-8");
    return add_body(std::move(body));
}

RequestBuilder& RequestBuilder::accept_json()
{
    return add_header("Accept: Application/json");
}

RequestBuilder& RequestBuilder::set_request_time_out(const long timeout)
{
    request_timeout = timeout;
    return *this;
}

RequestBuilder& RequestBuilder::set_connect_timeout(const long timeout)
{
    connect_timeout = timeout;
    return *this;
}

RequestBuilder& RequestBuilder::process_response(completion_fn_t fn)
{
    assert(!is_built);
    completion = std::move(fn);
    return *this;
}

RequestBuilder& RequestBuilder::set_custom_read_handler(size_t (*handler)(char *, size_t , size_t , void *), void *userdata)
{
    options->set_option(CURLOPT_READFUNCTION, handler);
    options->set_option(CURLOPT_READDATA, userdata);
    have_data_out = true;
    return *this;
}

RequestBuilder& RequestBuilder::set_custom_write_handler(size_t (*handler)(char *, size_t , size_t , void *), void *userdata)
{
    options->set_option(CURLOPT_WRITEFUNCTION,handler);
    options->set_option(CURLOPT_WRITEDATA, userdata);
    have_data_in = true;
    return *this;
}

void RequestBuilder::build()
{
    if (!is_built)
    {
        if (!have_data_in)
        {
            this->store_response(request->get_default_input_buffer());
        }

        if (!have_header_in)
        {
            this->store_header(request->get_default_header_input_buffer());
        }

        if (have_data_out)
        {
            options->set_option(CURLOPT_UPLOAD, 1L);
        }

        if (request_timeout >= 0)
        {
            options->set_option(CURLOPT_TIMEOUT_MS, request_timeout);
        }

        if (connect_timeout >= 0)
        {
            options->set_option(CURLOPT_CONNECTTIMEOUT_MS, connect_timeout);
        }

        if (request->get_headers())
        {
            options->set_option(CURLOPT_HTTPHEADER, request->get_headers());
        }

        options->set_option(CURLOPT_URL, url);

        request->prepare(request_type, std::move(completion));
        is_built = true;
    }
}

void RequestBuilder::send_request()
{
    build();
    request->execute();
}