/**
	curl-asio: wrapper for integrating libcurl with boost.asio applications
	Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
	See COPYING for license information.
*/

#pragma once

#define CURL_STATICLIB
#define CURLASIO_STATIC

#if !defined CURLASIO_API
#	if defined _MSC_VER
#		if defined CURLASIO_STATIC
#			define CURLASIO_API
#		elif defined libcurlasio_EXPORTS
#			define CURLASIO_API __declspec(dllexport)
#		else
#			define CURLASIO_API __declspec(dllimport)
#		endif
#	else
#		define CURLASIO_API
#	endif
#endif


#if defined(_MSC_VER)
#include <WinSock2.h>
#endif

#define ASIO_STANDALONE
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_TYPE_TRAITS
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_CHRONO