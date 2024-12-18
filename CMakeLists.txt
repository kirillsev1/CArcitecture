cmake_minimum_required(VERSION 3.12)
project(service_template CXX)


# Adding userver dependency
find_package(userver COMPONENTS core postgresql kafka grpc QUIET)
if(NOT userver_FOUND)  # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_KAFKA ON CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif()
endif()

add_subdirectory(third_party/cpp-jwt)

userver_setup_environment()


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
        src/api/v1/hello/hello.hpp
        src/api/v1/hello/hello.cpp
        src/api/v1/user/register.hpp
        src/api/v1/user/register.cpp
        src/api/v1/user/info.cpp
        src/api/v1/user/info.hpp
        src/api/v1/user/login.cpp
        src/api/v1/user/login.hpp
        src/dto/user.cpp
        src/dto/user.hpp
        src/db/types.hpp
        src/models/user.cpp
        src/models/user.hpp
        src/utils/jwt.cpp
        src/utils/jwt.hpp
        src/api/v1/photo/load.cpp
        src/api/v1/photo/load.hpp
        src/api/v1/photo/produce.cpp
        src/api/v1/photo/produce.hpp
        src/api/v1/photo/consume.cpp
        src/api/v1/photo/consume.hpp
        src/api/v1/photo/consumer.cpp
        src/api/v1/photo/consumer.hpp
)
include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::core userver::postgresql userver::kafka userver::grpc)
target_include_directories(${PROJECT_NAME}_objs PUBLIC cpp-jwt)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC cpp-jwt)

# Create a proto library with userver extensions
userver_add_grpc_library(${PROJECT_NAME}-proto PROTOS service_example.proto)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC ${PROJECT_NAME}-proto)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)


# Unit Tests
add_executable(${PROJECT_NAME}_unittest
    src/api/v1/hello/hello_test.cpp
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark
    src/api/v1/hello/hello_benchmark.cpp
)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver::ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)


# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple()


# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

set(CONFIG_JWT ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/jwt_config.json)

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
