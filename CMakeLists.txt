### CMake Settings ###

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0028 NEW)

### External Libraries ###

## Definitions ##

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH <comment> FORCE)
set(REACTIVEX_PROJECT_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(CMAKE_CXX_STANDARD 17)
project(ReactiveCurl VERSION 1.0.0 LANGUAGES CXX)
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    project(reactivex VERSION 1.0.0 LANGUAGES CXX)
    set(CMAKE_ANDROID_BUILD_ARGS
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NDK=${ANDROID_NDK}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_BUILD_TYPE=Release
        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
        -DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN}
    )
else()
    set(CMAKE_ANDROID_BUILD_ARGS "")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_CURL_WINDOWS_BUILD_ARGS
        -DUSE_WIN32_CRYPTO=1
    )
else()
    set(CMAKE_CURL_WINDOWS_BUILD_ARGS "")
endif()

include(ExternalProject)

## Download and Build Libraries ##

# 1-Nlomann/json #

ExternalProject_Add(nlohmann
	URL "https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.zip"
	CMAKE_ARGS
		-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
		-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
		-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
		-DJSON_BuildTests=OFF
        -DBUILD_TESTING=OFF
		-DJSON_MultipleHeaders=OFF
        ${CMAKE_ANDROID_BUILD_ARGS}
)

# 2-Jbeder/yaml-cpp #

ExternalProject_Add(yamlcpp
    DEPENDS nlohmann
    URL "https://github.com/mhalikhani94/yaml-cpp/archive/refs/tags/v0.0.1.zip"
    BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cpp.a
    CMAKE_ARGS
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DYAML_BUILD_SHARED_LIBS=OFF
        -DYAML_CPP_BUILD_TESTS=OFF
        ${CMAKE_ANDROID_BUILD_ARGS}
)

add_library(yamlcpp_lib STATIC IMPORTED GLOBAL)
add_dependencies(yamlcpp_lib yamlcpp)
set_target_properties(yamlcpp_lib
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cpp.a
)

add_custom_command(TARGET yamlcpp POST_BUILD
        COMMAND "${ANDROID_TOOLCHAIN_PREFIX}strip" -g -S -d --verbose
        ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cpp.a
        COMMENT "."
)

# 3-Madler/zlib #

ExternalProject_Add(zlib
        DEPENDS yamlcpp
        URL "https://github.com/mhalikhani94/zlib/archive/refs/tags/v0.0.1.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libz.a
        CMAKE_ARGS
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=OFF
            -DSKIP_INSTALL_FILES=ON
            ${CMAKE_ANDROID_BUILD_ARGS}
)
add_library(zlib_lib STATIC IMPORTED GLOBAL)
    add_dependencies(zlib_lib zlib)
    set_target_properties(zlib_lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libz.a
    )

add_custom_command(TARGET zlib POST_BUILD
        COMMAND "${ANDROID_TOOLCHAIN_PREFIX}strip" -g -S -d --verbose
        ${CMAKE_INSTALL_PREFIX}/lib/libz.a
        COMMENT "."
)

# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#     add_library(zlib_lib STATIC IMPORTED GLOBAL)
#     add_dependencies(zlib_lib zlib)
#     set_target_properties(zlib_lib
#         PROPERTIES IMPORTED_LOCATION
#         ${CMAKE_INSTALL_PREFIX}/lib/libzlibstatic.a
# )
# else()
#     add_library(zlib_lib STATIC IMPORTED GLOBAL)
#     add_dependencies(zlib_lib zlib)
#     set_target_properties(zlib_lib
#         PROPERTIES IMPORTED_LOCATION
#         ${CMAKE_INSTALL_PREFIX}/lib/libz.a
#     )
# endif()   


# 4-Janbar/openssl-cmake #

ExternalProject_Add(openssl
        DEPENDS zlib
        URL "https://github.com/mhalikhani94/openssl-cmake/archive/refs/tags/v0.0.4.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libssl.a
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libcrypto.a
        CMAKE_ARGS
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            -DWITH_APPS=OFF
            ${CMAKE_ANDROID_BUILD_ARGS}
)
set(OPENSSL_ROOT_DIR ${CMAKE_INSTALL_PREFIX}/ssl)
set(OPENSSL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(OPENSSL_SSL_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libssl.a)
set(OPENSSL_CRYPTO_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcrypto.a)

add_library(ssl_lib STATIC IMPORTED GLOBAL)
add_dependencies(ssl_lib openssl)
set_target_properties(ssl_lib
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_INSTALL_PREFIX}/lib/libssl.a
)

add_library(crypto_lib STATIC IMPORTED GLOBAL)
add_dependencies(crypto_lib openssl)
set_target_properties(crypto_lib
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_INSTALL_PREFIX}/lib/libcrypto.a
)

add_custom_command(TARGET openssl POST_BUILD
        COMMAND "${ANDROID_TOOLCHAIN_PREFIX}strip" -g -S -d --verbose
        ${CMAKE_INSTALL_PREFIX}/lib/libssl.a
        COMMENT "."
)

add_custom_command(TARGET openssl POST_BUILD
        COMMAND "${ANDROID_TOOLCHAIN_PREFIX}strip" -g -S -d --verbose
        ${CMAKE_INSTALL_PREFIX}/lib/libcrypto.a
        COMMENT "."
)

add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
set_target_properties(OpenSSL::SSL PROPERTIES IMPORTED_LOCATION ${OPENSSL_SSL_LIBRARIES})
set_target_properties(OpenSSL::Crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_CRYPTO_LIBRARIES})

# 5-Curl/curl #

ExternalProject_Add(curl
        DEPENDS openssl
        URL "https://github.com/mhalikhani94/curl/archive/refs/tags/v0.0.4.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libcurl.a
        CMAKE_ARGS
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            -DCURL_USE_OPENSSL=ON
            -DCURL_STATICLIB=1
            -DBUILD_SHARED_LIBS=0
            -DCURL_USE_LIBPSL=OFF
            -DUSE_LIBIDN2=OFF
            -DCURL_USE_LIBSSH2=OFF
            -DOPENSSL_SSL_LIBRARIES=${OPENSSL_SSL_LIBRARIES}
            -DOPENSSL_CRYPTO_LIBRARIES=${OPENSSL_CRYPTO_LIBRARIES}
            -DOPENSSL_LIBRARIES=${OPENSSL_LIBRARIES}
            -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
            -DOPENSSL_USE_STATIC_LIBS=TRUE
            -DBUILD_CURL_EXE=0
            -DBUILD_TESTING=OFF
            -DUSE_MANUAL=OFF
            ${CMAKE_CURL_WINDOWS_BUILD_ARGS}
            ${CMAKE_ANDROID_BUILD_ARGS}
)

add_library(curl_lib STATIC IMPORTED)
add_dependencies(curl_lib curl)
set_target_properties(curl_lib
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_INSTALL_PREFIX}/lib/libcurl.a
)

add_custom_command(TARGET curl POST_BUILD
        COMMAND "${ANDROID_TOOLCHAIN_PREFIX}strip" -g -S -d --verbose
        ${CMAKE_INSTALL_PREFIX}/lib/libcurl.a
        COMMENT "."
)

### Application ###

set(HTTP_CLIENT_FILES
    src/http-client/curl-handler.hpp
    src/http-client/exceptions.hpp
    src/http-client/http-client.hpp
    src/http-client/http-request.hpp
    src/http-client/http-request.cpp
    src/http-client/http-response.hpp
    src/http-client/options.hpp
    src/http-client/request-builder.hpp
    src/http-client/request-builder.cpp
)

set(CARDANO_BLOCKCHAIN_API_FILES
    src/blockchains/cardano/cardano-api.hpp
    src/blockchains/cardano/cardano-api.cpp
    src/blockchains/cardano/cardano-types.hpp
)

set(BITCOIN_BLOCKCHAIN_API_FILES
    src/blockchains/bitcoin/bitcoin-api.hpp
    src/blockchains/bitcoin/bitcoin-api.cpp
    src/blockchains/bitcoin/bitcoin-types.hpp
)

set(BLOCKCHAIN_API_FILES
    src/blockchains/cardano/cardano-api.hpp
    src/blockchains/cardano/cardano-api.cpp
    src/blockchains/cardano/cardano-types.hpp
    src/blockchains/cardano/cardano-parsers.hpp
    src/blockchains/cardano/cardano-parsers.cpp
    src/blockchains/bitcoin/bitcoin-api.hpp
    src/blockchains/bitcoin/bitcoin-api.cpp
    src/blockchains/bitcoin/bitcoin-types.hpp
    src/blockchains/ethereum/ethereum-api.hpp
    src/blockchains/ethereum/ethereum-api.cpp
    src/blockchains/ethereum/ethereum-types.hpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    include_directories(${PROJECT_SOURCE_DIR}/include)

    add_library(reactivex
        SHARED
        native-lib.cpp src/types.hpp ${HTTP_CLIENT_FILES} ${BLOCKCHAIN_API_FILES}
    )

    add_dependencies(reactivex zlib openssl curl nlohmann)

    find_library(log-lib
        log
    )

    target_include_directories(reactivex
        PUBLIC
        ${REACTIVEX_PROJECT_INCLUDE_DIR}
    )

    target_link_libraries(reactivex
        ${log-lib}
        curl_lib
        ssl_lib
        crypto_lib
        zlib_lib
    )
else()

    include_directories(${PROJECT_SOURCE_DIR}/include)

    add_library(ReactiveCurl
        SHARED
        src/types.hpp ${HTTP_CLIENT_FILES} ${BLOCKCHAIN_API_FILES}
    )

    add_dependencies(ReactiveCurl zlib yamlcpp openssl curl nlohmann)

    target_include_directories(ReactiveCurl
        PUBLIC
        ${REACTIVEX_PROJECT_INCLUDE_DIR}
    )

    target_link_libraries(ReactiveCurl
        ${log-lib}
        curl_lib
        ssl_lib
        crypto_lib
        zlib_lib
    )

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(ReactiveCurl
        curl_lib
        ssl_lib 
        crypto_lib
        zlib_lib
        yamlcpp_lib
        wldap32 ws2_32 Crypt32 Normaliz
    )
    else()
    target_link_libraries(ReactiveCurl
        curl_lib
        zlib_lib
        ssl_lib
        crypto_lib
        yamlcpp_lib
        dl pthread
    )
    endif()

    # add_executable(ReactiveCurl src/reactive-curl.cpp src/types.hpp ${HTTP_CLIENT_FILES} ${BLOCKCHAIN_API_FILES} )

    # add_dependencies(ReactiveCurl curl openssl zlib yamlcpp nlohmann)

    # include_directories("include")
    # target_include_directories(ReactiveCurl PUBLIC
    # ${REACTIVEX_PROJECT_INCLUDE_DIR}
    # )
    # if(WIN32)
    # target_link_libraries(ReactiveCurl
    # curl_lib
    # ssl_lib 
    # crypto_lib
    # zlib_lib
    # yamlcpp_lib
    # wldap32 ws2_32 Crypt32 Normaliz
    # )
    # else()
    # target_link_libraries(ReactiveCurl
    # curl_lib
    # zlib_lib
    # ssl_lib
    # crypto_lib
    # yamlcpp_lib
    # dl pthread
    # )
    # endif(WIN32)

endif()