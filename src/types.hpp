#ifndef REACTIVE_CURL_TYPES_HPP
#define REACTIVE_CURL_TYPES_HPP

#define CURL_STATICLIB
#define USE_WIN32_CRYPTO
#include <curl/curl.h>

#include <string>
#include <map>
#include <assert.h>

enum class RequestType
{
	kGet,
	kPut,
	kPost,
	kHead,
	kDelete,
	kPatch,
	kOptions,
	kInvalid
};

#endif //REACTIVE_CURL_TYPES_HPP