cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/src/webfr/core/base/Environment.hpp" WEBFR_VERSION_MACRO REGEX "#define WEBFR_VERSION \"[0-9]+.[0-9]+.[0-9]+\"$")
string(REGEX REPLACE "#define WEBFR_VERSION \"([0-9]+.[0-9]+.[0-9]+)\"$" "\\1" webfr_VERSION "${WEBFR_VERSION_MACRO}")

set(WEBFR_THIS_MODULE_NAME webfr)
set(WEBFR_THIS_MODULE_VERSION ${webfr_VERSION}) 
set(WEBFR_THIS_MODULE_LIBRARIES webfr webfr-test) 
set(WEBFR_THIS_MODULE_TARGETS webfr webfr-test) 
set(WEBFR_THIS_MODULE_DIRECTORIES webfr webfr-test) 

project(webfr VERSION ${WEBFR_THIS_MODULE_VERSION} LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(WEBFR_INSTALL "Create installation target for oat++" ON)
option(WEBFR_BUILD_TESTS "Create test target for oat++" ON)
option(WEBFR_LINK_ATOMIC "Link atomic library for other platform than MSVC|MINGW|APPLE|FreeBSD" ON)
option(WEBFR_MSVC_LINK_STATIC_RUNTIME "MSVC: Link with static runtime (/MT and /MTd)." OFF)

option(WEBFR_DISABLE_ENV_OBJECT_COUNTERS "Disable object counting for Release builds for better performance" OFF)
option(WEBFR_DISABLE_POOL_ALLOCATIONS "This will make webfr::base::memory::MemoryPool, method obtain and free call new and delete directly" OFF)

set(WEBFR_THREAD_HARDWARE_CONCURRENCY "AUTO" CACHE STRING "Predefined value for function webfr::concurrency::Thread::getHardwareConcurrency()")
set(WEBFR_THREAD_DISTRIBUTED_MEM_POOL_SHARDS_COUNT "10" CACHE STRING "Number of shards of ThreadDistributedMemoryPool")

option(WEBFR_COMPAT_BUILD_NO_THREAD_LOCAL "Disable 'thread_local' feature" OFF)
option(WEBFR_COMPAT_BUILD_NO_SET_AFFINITY "No 'pthread_setaffinity_np' method" OFF)

option(WEBFR_DISABLE_LOGV "DISABLE logs priority V" OFF)
option(WEBFR_DISABLE_LOGD "DISABLE logs priority D" OFF)
option(WEBFR_DISABLE_LOGI "DISABLE logs priority I" OFF)
option(WEBFR_DISABLE_LOGW "DISABLE logs priority W" OFF)
option(WEBFR_DISABLE_LOGE "DISABLE logs priority E" OFF)

message("\n############################################################################")
message("## webfr module compilation config:\n")

message("WEBFR_DISABLE_ENV_OBJECT_COUNTERS=${WEBFR_DISABLE_ENV_OBJECT_COUNTERS}")
message("WEBFR_DISABLE_POOL_ALLOCATIONS=${WEBFR_DISABLE_POOL_ALLOCATIONS}")
message("WEBFR_THREAD_HARDWARE_CONCURRENCY=${WEBFR_THREAD_HARDWARE_CONCURRENCY}")
message("WEBFR_THREAD_DISTRIBUTED_MEM_POOL_SHARDS_COUNT=${WEBFR_THREAD_DISTRIBUTED_MEM_POOL_SHARDS_COUNT}")

message("WEBFR_COMPAT_BUILD_NO_THREAD_LOCAL=${WEBFR_COMPAT_BUILD_NO_THREAD_LOCAL}")


if(WEBFR_DISABLE_ENV_OBJECT_COUNTERS)
    add_definitions(-DWEBFR_DISABLE_ENV_OBJECT_COUNTERS)
endif()

if(WEBFR_DISABLE_POOL_ALLOCATIONS)
    add_definitions (-DWEBFR_DISABLE_POOL_ALLOCATIONS)
endif()

set(AUTO_VALUE AUTO)
if(NOT WEBFR_THREAD_HARDWARE_CONCURRENCY STREQUAL AUTO_VALUE)
    add_definitions (-DWEBFR_THREAD_HARDWARE_CONCURRENCY=${WEBFR_THREAD_HARDWARE_CONCURRENCY})
endif()

add_definitions (
    -DWEBFR_THREAD_DISTRIBUTED_MEM_POOL_SHARDS_COUNT=${WEBFR_THREAD_DISTRIBUTED_MEM_POOL_SHARDS_COUNT}
)

if(WEBFR_COMPAT_BUILD_NO_THREAD_LOCAL)
    add_definitions(-DWEBFR_COMPAT_BUILD_NO_THREAD_LOCAL)
endif()

if(WEBFR_COMPAT_BUILD_NO_SET_AFFINITY)
    add_definitions(-DWEBFR_COMPAT_BUILD_NO_SET_AFFINITY)
endif()

if(WEBFR_DISABLE_LOGV)
    add_definitions(-DWEBFR_DISABLE_LOGV)
endif()

if(WEBFR_DISABLE_LOGD)
    add_definitions(-DWEBFR_DISABLE_LOGD)
endif()

if(WEBFR_DISABLE_LOGI)
    add_definitions(-DWEBFR_DISABLE_LOGI)
endif()

if(WEBFR_DISABLE_LOGW)
    add_definitions(-DWEBFR_DISABLE_LOGW)
endif()

if(WEBFR_DISABLE_LOGE)
    add_definitions(-DWEBFR_DISABLE_LOGE)
endif()

if(CMAKE_COMPILER_IS_GNUCXX AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 5.0)
    add_definitions(-DWEBFR_DISABLE_STD_PUT_TIME)
endif()

message("\n############################################################################\n")

message("webfr version: '${WEBFR_THIS_MODULE_VERSION}'")

include(cmake/msvc-runtime.cmake)
configure_msvc_runtime()

add_subdirectory(src)

if(WEBFR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()