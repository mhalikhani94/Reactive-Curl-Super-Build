### CMake Settings ###

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0028 NEW)

### External Libraries ###

## Definitions ##

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH <comment> FORCE)
set(REACTIVEX_PROJECT_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(CMAKE_CXX_STANDARD 17)
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    project(reactivex VERSION 1.0.0 LANGUAGES CXX)
    set(CMAKE_ANDROID_BUILD_ARGS
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NDK=${ANDROID_NDK}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
        -DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN}
    )
else()
    set(CMAKE_ANDROID_BUILD_ARGS "")
endif()

include(ExternalProject)

## Download and Build Libraries ##

# 1-Nlomann/json #

ExternalProject_Add(nlohmann
	URL "https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.zip"
	CMAKE_ARGS
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
		-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
		-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
		-DJSON_BuildTests=OFF
		-DJSON_MultipleHeaders=OFF
        ${CMAKE_ANDROID_BUILD_ARGS}
)

# 2-Jbeder/yaml-cpp #

ExternalProject_Add(yamlcpp
    DEPENDS nlohmann
    URL "https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.7.0.zip"
    BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cppd.a
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DYAML_BUILD_SHARED_LIBS=OFF
        -DYAML_CPP_BUILD_TESTS=OFF
        ${CMAKE_ANDROID_BUILD_ARGS}
)
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_library(yamlcpp_lib STATIC IMPORTED GLOBAL)
    add_dependencies(yamlcpp_lib yamlcpp)
    set_target_properties(yamlcpp_lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cppd.a
    )
else()
    set(YAMLCPP_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cpp.a)
endif()

# 3-Madler/zlib #

ExternalProject_Add(zlib
        DEPENDS yamlcpp
        URL "https://github.com/madler/zlib/archive/refs/tags/v1.2.12.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libz.a
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=OFF
            ${CMAKE_ANDROID_BUILD_ARGS}
)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(ZLIB_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libzlibstatic.a)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_library(zlib_lib STATIC IMPORTED GLOBAL)
    add_dependencies(zlib_lib zlib)
    set_target_properties(zlib_lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libz.a
    )    
else()
    set(ZLIB_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libz.a)
endif()

# 4-Janbar/openssl-cmake #

ExternalProject_Add(openssl
        DEPENDS zlib
        URL "https://github.com/mhalikhani94/openssl-cmake/archive/refs/tags/v0.0.1.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libssl_1_1.a
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libcrypto_1_1.a
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            -DWITH_APPS=OFF
            ${CMAKE_ANDROID_BUILD_ARGS}
)
set(OPENSSL_ROOT_DIR ${CMAKE_INSTALL_PREFIX}/ssl)
set(OPENSSL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(OPENSSL_SSL_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libssl_1_1.a)
    set(OPENSSL_CRYPTO_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcrypto_1_1.a)
    
    add_library(ssl_lib STATIC IMPORTED GLOBAL)
    add_dependencies(ssl_lib openssl)
    set_target_properties(ssl_lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libssl_1_1.a
    )

    add_library(crypto_lib STATIC IMPORTED GLOBAL)
    add_dependencies(crypto_lib openssl)
    set_target_properties(crypto_lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libcrypto_1_1.a
    )
else()
    set(OPENSSL_SSL_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libssl.a)
    set(OPENSSL_CRYPTO_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcrypto.a)
endif()
add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
set_target_properties(OpenSSL::SSL PROPERTIES IMPORTED_LOCATION ${OPENSSL_SSL_LIBRARIES})
set_target_properties(OpenSSL::Crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_CRYPTO_LIBRARIES})

# 5-Curl/curl #

ExternalProject_Add(curl
        DEPENDS openssl
        URL "https://github.com/mhalikhani94/curl/archive/refs/tags/v0.0.2.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libcurl-d.a
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            -DCURL_USE_OPENSSL=ON
            -DCURL_STATICLIB=1
            -DBUILD_SHARED_LIBS=0
            -DCURL_USE_LIBPSL=OFF
            -DUSE_LIBIDN2=OFF
            -DCURL_USE_LIBSSH2=OFF
            -DCURL_DISABLE_LDAP=ON
            -DOPENSSL_SSL_LIBRARIES=${OPENSSL_SSL_LIBRARIES}
            -DOPENSSL_CRYPTO_LIBRARIES=${OPENSSL_CRYPTO_LIBRARIES}
            -DOPENSSL_LIBRARIES=${OPENSSL_LIBRARIES}
            -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
            -DOPENSSL_USE_STATIC_LIBS=TRUE
            -DBUILD_CURL_EXE=0
            -DBUILD_TESTING=OFF
            ${CMAKE_ANDROID_BUILD_ARGS}
)
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_library(curl_lib STATIC IMPORTED GLOBAL)
    add_dependencies(curl_lib curl)
    set_target_properties(curl_lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libcurl-d.a
    )    
else()
    set(CURL_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcurl.a)
endif()

### Application ###

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    include_directories(${PROJECT_SOURCE_DIR}/include)

    add_library(reactivex
        SHARED
        native-lib.cpp rx-request-manager.cpp rx-request-manager.hpp rx-curl.hpp
    )

    add_dependencies(reactivex yamlcpp zlib openssl curl nlohmann)

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
        yamlcpp_lib
    )
else()

    ExternalProject_Add(reactive-curl
        DEPENDS curl
        URL "https://github.com/mhalikhani94/Reactive-Curl/archive/refs/tags/v0.0.5.zip"
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
            -DCURL_INCLUDE_DIRS=${REACTIVEX_PROJECT_INCLUDE_DIR}
            -DOPENSSL_INCLUDE_DIR=${REACTIVEX_PROJECT_INCLUDE_DIR}
            -DZLIB_INCLUDE_DIR=${REACTIVEX_PROJECT_INCLUDE_DIR}
            -DJSONCPP_INCLUDE_DIR=${REACTIVEX_PROJECT_INCLUDE_DIR}
            -DYAMLCPP_INCLUDE_DIR=${REACTIVEX_PROJECT_INCLUDE_DIR}
            -DYAMLCPP_LIBRARIES=${YAMLCPP_LIBRARIES}
            -DCURL_LIBRARIES=${CURL_LIBRARIES}
            -DZLIB_LIBRARIES=${ZLIB_LIBRARIES}
            -DOPENSSL_SSL_LIBRARIES=${OPENSSL_SSL_LIBRARIES}
            -DOPENSSL_CRYPTO_LIBRARIES=${OPENSSL_CRYPTO_LIBRARIES}
            -DOPENSSL_USE_STATIC_LIBS=TRUE
    )
endif()