
add_executable(webfrAllTests
        webfr/AllTestsMain.cpp
        webfr/core/async/LockTest.cpp
        webfr/core/async/LockTest.hpp
        webfr/core/base/CommandLineArgumentsTest.cpp
        webfr/core/base/CommandLineArgumentsTest.hpp
        webfr/core/base/LoggerTest.cpp
        webfr/core/base/LoggerTest.hpp
        webfr/core/base/collection/LinkedListTest.cpp
        webfr/core/base/collection/LinkedListTest.hpp
        webfr/core/base/memory/MemoryPoolTest.cpp
        webfr/core/base/memory/MemoryPoolTest.hpp
        webfr/core/base/memory/PerfTest.cpp
        webfr/core/base/memory/PerfTest.hpp
        webfr/core/data/buffer/ProcessorTest.cpp
        webfr/core/data/buffer/ProcessorTest.hpp
        webfr/core/data/mapping/type/AnyTest.cpp
        webfr/core/data/mapping/type/AnyTest.hpp
        webfr/core/data/mapping/type/EnumTest.cpp
        webfr/core/data/mapping/type/EnumTest.hpp
        webfr/core/data/mapping/type/InterpretationTest.cpp
        webfr/core/data/mapping/type/InterpretationTest.hpp
        webfr/core/data/mapping/type/ListTest.cpp
        webfr/core/data/mapping/type/ListTest.hpp
        webfr/core/data/mapping/type/ObjectTest.cpp
        webfr/core/data/mapping/type/ObjectTest.hpp
        webfr/core/data/mapping/type/ObjectWrapperTest.cpp
        webfr/core/data/mapping/type/ObjectWrapperTest.hpp
        webfr/core/data/mapping/type/PairListTest.cpp
        webfr/core/data/mapping/type/PairListTest.hpp
        webfr/core/data/mapping/type/PrimitiveTest.cpp
        webfr/core/data/mapping/type/PrimitiveTest.hpp
        webfr/core/data/mapping/type/StringTest.cpp
        webfr/core/data/mapping/type/StringTest.hpp
        webfr/core/data/mapping/type/TypeTest.cpp
        webfr/core/data/mapping/type/TypeTest.hpp
        webfr/core/data/mapping/type/UnorderedMapTest.cpp
        webfr/core/data/mapping/type/UnorderedMapTest.hpp
        webfr/core/data/mapping/type/UnorderedSetTest.cpp
        webfr/core/data/mapping/type/UnorderedSetTest.hpp
        webfr/core/data/mapping/type/VectorTest.cpp
        webfr/core/data/mapping/type/VectorTest.hpp
        webfr/core/data/mapping/TypeResolverTest.cpp
        webfr/core/data/mapping/TypeResolverTest.hpp
        webfr/core/data/share/LazyStringMapTest.cpp
        webfr/core/data/share/LazyStringMapTest.hpp
        webfr/core/data/share/MemoryLabelTest.cpp
        webfr/core/data/share/MemoryLabelTest.hpp
        webfr/core/data/share/StringTemplateTest.cpp
        webfr/core/data/share/StringTemplateTest.hpp
        webfr/core/data/stream/BufferStreamTest.cpp
        webfr/core/data/stream/BufferStreamTest.hpp
        webfr/core/data/stream/ChunkedBufferTest.cpp
        webfr/core/data/stream/ChunkedBufferTest.hpp
        webfr/core/parser/CaretTest.cpp
        webfr/core/parser/CaretTest.hpp
        webfr/core/provider/PoolTest.cpp
        webfr/core/provider/PoolTest.hpp
        webfr/encoding/Base64Test.cpp
        webfr/encoding/Base64Test.hpp
        webfr/encoding/UnicodeTest.cpp
        webfr/encoding/UnicodeTest.hpp
        webfr/network/ConnectionPoolTest.cpp
        webfr/network/ConnectionPoolTest.hpp
        webfr/network/UrlTest.cpp
        webfr/network/UrlTest.hpp
        webfr/network/virtual_/InterfaceTest.cpp
        webfr/network/virtual_/InterfaceTest.hpp
        webfr/network/virtual_/PipeTest.cpp
        webfr/network/virtual_/PipeTest.hpp
        webfr/parser/json/mapping/DTOMapperPerfTest.cpp
        webfr/parser/json/mapping/DTOMapperPerfTest.hpp
        webfr/parser/json/mapping/DTOMapperTest.cpp
        webfr/parser/json/mapping/DTOMapperTest.hpp
        webfr/parser/json/mapping/DeserializerTest.cpp
        webfr/parser/json/mapping/DeserializerTest.hpp
        webfr/parser/json/mapping/EnumTest.cpp
        webfr/parser/json/mapping/EnumTest.hpp
        webfr/parser/json/mapping/UnorderedSetTest.cpp
        webfr/parser/json/mapping/UnorderedSetTest.hpp
        webfr/web/protocol/http/encoding/ChunkedTest.cpp
        webfr/web/protocol/http/encoding/ChunkedTest.hpp
        webfr/web/mime/multipart/StatefulParserTest.cpp
        webfr/web/mime/multipart/StatefulParserTest.hpp
        webfr/web/server/api/ApiControllerTest.cpp
        webfr/web/server/api/ApiControllerTest.hpp
        webfr/web/server/handler/AuthorizationHandlerTest.cpp
        webfr/web/server/handler/AuthorizationHandlerTest.hpp
        webfr/web/server/HttpRouterTest.cpp
        webfr/web/server/HttpRouterTest.hpp
        webfr/web/ClientRetryTest.cpp
        webfr/web/ClientRetryTest.hpp
        webfr/web/PipelineTest.cpp
        webfr/web/PipelineTest.hpp
        webfr/web/PipelineAsyncTest.cpp
        webfr/web/PipelineAsyncTest.hpp
        webfr/web/FullAsyncTest.cpp
        webfr/web/FullAsyncTest.hpp
        webfr/web/FullTest.cpp
        webfr/web/FullTest.hpp
        webfr/web/FullAsyncClientTest.cpp
        webfr/web/FullAsyncClientTest.hpp
        webfr/web/app/Client.hpp
        webfr/web/app/BearerAuthorizationController.hpp
        webfr/web/app/BasicAuthorizationController.hpp
        webfr/web/app/Controller.hpp
        webfr/web/app/ControllerAsync.hpp
        webfr/web/app/DTOs.hpp
        webfr/web/app/ControllerWithInterceptors.hpp
        webfr/web/app/ControllerWithInterceptorsAsync.hpp
)

target_link_libraries(webfrAllTests PRIVATE webfr PRIVATE webfr-test)

set_target_properties(webfrAllTests PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(webfrAllTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_test(webfrAllTests webfrAllTests)
