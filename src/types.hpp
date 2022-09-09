#ifndef REACTIVE_CURL_TYPES_HPP
#define REACTIVE_CURL_TYPES_HPP

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