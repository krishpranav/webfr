add_library(webfr
        webfr/algorithm/CRC.cpp
        webfr/algorithm/CRC.hpp
        webfr/codegen/ApiClient_define.hpp
        webfr/codegen/ApiClient_undef.hpp
        webfr/codegen/api_controller/base_define.hpp
        webfr/codegen/api_controller/base_undef.hpp
        webfr/codegen/api_controller/auth_define.hpp
        webfr/codegen/api_controller/auth_undef.hpp
        webfr/codegen/api_controller/cors_define.hpp
        webfr/codegen/api_controller/cors_undef.hpp
        webfr/codegen/ApiController_define.hpp
        webfr/codegen/ApiController_undef.hpp
        webfr/codegen/DbClient_define.hpp
        webfr/codegen/DbClient_undef.hpp
        webfr/codegen/dto/base_define.hpp
        webfr/codegen/dto/base_undef.hpp
        webfr/codegen/dto/enum_define.hpp
        webfr/codegen/dto/enum_undef.hpp
        webfr/codegen/DTO_define.hpp
        webfr/codegen/DTO_undef.hpp
        webfr/core/Types.hpp
        webfr/core/async/Coroutine.cpp
        webfr/core/async/Coroutine.hpp
        webfr/core/async/CoroutineWaitList.cpp
        webfr/core/async/CoroutineWaitList.hpp
        webfr/core/async/Error.cpp
        webfr/core/async/Error.hpp
        webfr/core/async/Executor.cpp
        webfr/core/async/Executor.hpp
        webfr/core/async/Lock.cpp
        webfr/core/async/Lock.hpp
        webfr/core/async/Processor.cpp
        webfr/core/async/Processor.hpp
        webfr/core/async/worker/Worker.cpp
        webfr/core/async/worker/Worker.hpp
        webfr/core/async/worker/IOEventWorker_common.cpp
        webfr/core/async/worker/IOEventWorker_kqueue.cpp
        webfr/core/async/worker/IOEventWorker_epoll.cpp
        webfr/core/async/worker/IOEventWorker_stub.cpp
        webfr/core/async/worker/IOEventWorker.hpp
        webfr/core/async/worker/IOWorker.cpp
        webfr/core/async/worker/IOWorker.hpp
        webfr/core/async/worker/TimerWorker.cpp
        webfr/core/async/worker/TimerWorker.hpp
        webfr/core/base/CommandLineArguments.cpp
        webfr/core/base/CommandLineArguments.hpp
        webfr/core/base/Config.hpp
        webfr/core/base/Countable.cpp
        webfr/core/base/Countable.hpp
        webfr/core/base/Environment.cpp
        webfr/core/base/Environment.hpp
        webfr/core/base/ObjectHandle.hpp
        webfr/core/base/StrBuffer.cpp
        webfr/core/base/StrBuffer.hpp
        webfr/core/base/memory/Allocator.cpp
        webfr/core/base/memory/Allocator.hpp
        webfr/core/base/memory/MemoryPool.cpp
        webfr/core/base/memory/MemoryPool.hpp
        webfr/core/base/memory/ObjectPool.cpp
        webfr/core/base/memory/ObjectPool.hpp
        webfr/core/collection/FastQueue.hpp
        webfr/core/collection/LinkedList.hpp
        webfr/core/collection/ListMap.hpp
        webfr/core/concurrency/SpinLock.cpp
        webfr/core/concurrency/SpinLock.hpp
        webfr/core/concurrency/Thread.cpp
        webfr/core/concurrency/Thread.hpp
        webfr/core/IODefinitions.cpp
        webfr/core/IODefinitions.hpp
        webfr/core/data/buffer/FIFOBuffer.cpp
        webfr/core/data/buffer/FIFOBuffer.hpp
        webfr/core/data/buffer/IOBuffer.cpp
        webfr/core/data/buffer/IOBuffer.hpp
        webfr/core/data/buffer/Processor.cpp
        webfr/core/data/buffer/Processor.hpp
        webfr/core/data/mapping/ObjectMapper.cpp
        webfr/core/data/mapping/ObjectMapper.hpp
        webfr/core/data/mapping/TypeResolver.cpp
        webfr/core/data/mapping/TypeResolver.hpp
        webfr/core/data/mapping/type/Any.cpp
        webfr/core/data/mapping/type/Any.hpp
        webfr/core/data/mapping/type/Enum.cpp
        webfr/core/data/mapping/type/Enum.hpp
        webfr/core/data/mapping/type/List.cpp
        webfr/core/data/mapping/type/List.hpp
        webfr/core/data/mapping/type/PairList.cpp
        webfr/core/data/mapping/type/PairList.hpp
        webfr/core/data/mapping/type/Object.cpp
        webfr/core/data/mapping/type/Object.hpp
        webfr/core/data/mapping/type/Primitive.cpp
        webfr/core/data/mapping/type/Primitive.hpp
        webfr/core/data/mapping/type/Type.cpp
        webfr/core/data/mapping/type/Type.hpp
        webfr/core/data/mapping/type/UnorderedMap.cpp
        webfr/core/data/mapping/type/UnorderedMap.hpp
        webfr/core/data/mapping/type/UnorderedSet.cpp
        webfr/core/data/mapping/type/UnorderedSet.hpp
        webfr/core/data/mapping/type/Vector.cpp
        webfr/core/data/mapping/type/Vector.hpp
        webfr/core/data/share/LazyStringMap.hpp
        webfr/core/data/share/MemoryLabel.cpp
        webfr/core/data/share/MemoryLabel.hpp
        webfr/core/data/share/StringTemplate.cpp
        webfr/core/data/share/StringTemplate.hpp
        webfr/core/data/stream/BufferStream.cpp
        webfr/core/data/stream/BufferStream.hpp
        webfr/core/data/stream/ChunkedBuffer.cpp
        webfr/core/data/stream/ChunkedBuffer.hpp
        webfr/core/data/stream/FileStream.cpp
        webfr/core/data/stream/FileStream.hpp
        webfr/core/data/stream/Stream.cpp
        webfr/core/data/stream/Stream.hpp
        webfr/core/data/stream/StreamBufferedProxy.cpp
        webfr/core/data/stream/StreamBufferedProxy.hpp
        webfr/core/macro/basic.hpp
        webfr/core/macro/codegen.hpp
        webfr/core/macro/component.hpp
        webfr/core/parser/Caret.cpp
        webfr/core/parser/Caret.hpp
        webfr/core/parser/ParsingError.cpp
        webfr/core/parser/ParsingError.hpp
        webfr/core/provider/Pool.hpp
        webfr/core/provider/Provider.hpp
        webfr/core/utils/Binary.cpp
        webfr/core/utils/Binary.hpp
        webfr/core/utils/ConversionUtils.cpp
        webfr/core/utils/ConversionUtils.hpp
        webfr/core/utils/Random.cpp
        webfr/core/utils/Random.hpp
        webfr/encoding/Base64.cpp
        webfr/encoding/Base64.hpp
        webfr/encoding/Hex.cpp
        webfr/encoding/Hex.hpp
        webfr/encoding/Unicode.cpp
        webfr/encoding/Unicode.hpp
        webfr/network/tcp/client/ConnectionProvider.cpp
        webfr/network/tcp/client/ConnectionProvider.hpp
        webfr/network/tcp/server/ConnectionProvider.cpp
        webfr/network/tcp/server/ConnectionProvider.hpp
        webfr/network/tcp/Connection.cpp
        webfr/network/tcp/Connection.hpp
        webfr/network/virtual_/Interface.cpp
        webfr/network/virtual_/Interface.hpp
        webfr/network/virtual_/Pipe.cpp
        webfr/network/virtual_/Pipe.hpp
        webfr/network/virtual_/Socket.cpp
        webfr/network/virtual_/Socket.hpp
        webfr/network/virtual_/client/ConnectionProvider.cpp
        webfr/network/virtual_/client/ConnectionProvider.hpp
        webfr/network/virtual_/server/ConnectionProvider.cpp
        webfr/network/virtual_/server/ConnectionProvider.hpp
        webfr/network/Address.cpp
        webfr/network/Address.hpp
        webfr/network/ConnectionHandler.hpp
        webfr/network/ConnectionPool.cpp
        webfr/network/ConnectionPool.hpp
        webfr/network/ConnectionProvider.cpp
        webfr/network/ConnectionProvider.hpp
        webfr/network/Server.cpp
        webfr/network/Server.hpp
        webfr/network/Url.cpp
        webfr/network/Url.hpp
        webfr/orm/Connection.hpp
        webfr/orm/DbClient.cpp
        webfr/orm/DbClient.hpp
        webfr/orm/Executor.cpp
        webfr/orm/Executor.hpp
        webfr/orm/QueryResult.cpp
        webfr/orm/QueryResult.hpp
        webfr/orm/SchemaMigration.cpp
        webfr/orm/SchemaMigration.hpp
        webfr/orm/Transaction.cpp
        webfr/orm/Transaction.hpp
        webfr/parser/json/Beautifier.cpp
        webfr/parser/json/Beautifier.hpp
        webfr/parser/json/Utils.cpp
        webfr/parser/json/Utils.hpp
        webfr/parser/json/mapping/Deserializer.cpp
        webfr/parser/json/mapping/Deserializer.hpp
        webfr/parser/json/mapping/ObjectMapper.cpp
        webfr/parser/json/mapping/ObjectMapper.hpp
        webfr/parser/json/mapping/Serializer.cpp
        webfr/parser/json/mapping/Serializer.hpp
        webfr/web/client/ApiClient.cpp
        webfr/web/client/ApiClient.hpp
        webfr/web/client/HttpRequestExecutor.cpp
        webfr/web/client/HttpRequestExecutor.hpp
        webfr/web/client/RequestExecutor.cpp
        webfr/web/client/RequestExecutor.hpp
        webfr/web/client/RetryPolicy.cpp
        webfr/web/client/RetryPolicy.hpp
        webfr/web/mime/multipart/FileStreamProvider.cpp
        webfr/web/mime/multipart/FileStreamProvider.hpp
        webfr/web/mime/multipart/InMemoryPartReader.cpp
        webfr/web/mime/multipart/InMemoryPartReader.hpp
        webfr/web/mime/multipart/Multipart.cpp
        webfr/web/mime/multipart/Multipart.hpp
        webfr/web/mime/multipart/Part.cpp
        webfr/web/mime/multipart/Part.hpp
        webfr/web/mime/multipart/PartList.cpp
        webfr/web/mime/multipart/PartList.hpp
        webfr/web/mime/multipart/Reader.cpp
        webfr/web/mime/multipart/Reader.hpp
        webfr/web/mime/multipart/StatefulParser.cpp
        webfr/web/mime/multipart/StatefulParser.hpp
        webfr/web/mime/multipart/StreamPartReader.cpp
        webfr/web/mime/multipart/StreamPartReader.hpp
        webfr/web/protocol/CommunicationError.cpp
        webfr/web/protocol/CommunicationError.hpp
        webfr/web/protocol/http/Http.cpp
        webfr/web/protocol/http/Http.hpp
        webfr/web/protocol/http/incoming/BodyDecoder.cpp
        webfr/web/protocol/http/incoming/BodyDecoder.hpp
        webfr/web/protocol/http/incoming/Request.cpp
        webfr/web/protocol/http/incoming/Request.hpp
        webfr/web/protocol/http/incoming/RequestHeadersReader.cpp
        webfr/web/protocol/http/incoming/RequestHeadersReader.hpp
        webfr/web/protocol/http/incoming/Response.cpp
        webfr/web/protocol/http/incoming/Response.hpp
        webfr/web/protocol/http/incoming/ResponseHeadersReader.cpp
        webfr/web/protocol/http/incoming/ResponseHeadersReader.hpp
        webfr/web/protocol/http/incoming/SimpleBodyDecoder.cpp
        webfr/web/protocol/http/incoming/SimpleBodyDecoder.hpp
        webfr/web/protocol/http/outgoing/Body.cpp
        webfr/web/protocol/http/outgoing/Body.hpp
        webfr/web/protocol/http/outgoing/BufferBody.cpp
        webfr/web/protocol/http/outgoing/BufferBody.hpp
        webfr/web/protocol/http/outgoing/MultipartBody.cpp
        webfr/web/protocol/http/outgoing/MultipartBody.hpp
        webfr/web/protocol/http/outgoing/StreamingBody.cpp
        webfr/web/protocol/http/outgoing/StreamingBody.hpp
        webfr/web/protocol/http/outgoing/Request.cpp
        webfr/web/protocol/http/outgoing/Request.hpp
        webfr/web/protocol/http/outgoing/Response.cpp
        webfr/web/protocol/http/outgoing/Response.hpp
        webfr/web/protocol/http/outgoing/ResponseFactory.cpp
        webfr/web/protocol/http/outgoing/ResponseFactory.hpp
        webfr/web/protocol/http/encoding/Chunked.cpp
        webfr/web/protocol/http/encoding/Chunked.hpp
        webfr/web/protocol/http/encoding/ProviderCollection.cpp
        webfr/web/protocol/http/encoding/ProviderCollection.hpp
        webfr/web/protocol/http/encoding/EncoderProvider.hpp
        webfr/web/protocol/http/utils/CommunicationUtils.cpp
        webfr/web/protocol/http/utils/CommunicationUtils.hpp
        webfr/web/server/AsyncHttpConnectionHandler.cpp
        webfr/web/server/AsyncHttpConnectionHandler.hpp
        webfr/web/server/HttpConnectionHandler.cpp
        webfr/web/server/HttpConnectionHandler.hpp
        webfr/web/server/HttpProcessor.cpp
        webfr/web/server/HttpProcessor.hpp
        webfr/web/server/HttpRequestHandler.hpp
        webfr/web/server/HttpRouter.hpp
        webfr/web/server/api/ApiController.cpp
        webfr/web/server/api/ApiController.hpp
        webfr/web/server/api/Endpoint.cpp
        webfr/web/server/api/Endpoint.hpp
        webfr/web/server/handler/AuthorizationHandler.cpp
        webfr/web/server/handler/AuthorizationHandler.hpp
        webfr/web/server/handler/ErrorHandler.cpp
        webfr/web/server/handler/ErrorHandler.hpp
        webfr/web/server/interceptor/AllowCorsGlobal.cpp
        webfr/web/server/interceptor/AllowCorsGlobal.hpp
        webfr/web/server/interceptor/RequestInterceptor.hpp
        webfr/web/server/interceptor/ResponseInterceptor.hpp
        webfr/web/url/mapping/Pattern.cpp
        webfr/web/url/mapping/Pattern.hpp
        webfr/web/url/mapping/Router.hpp
)

set_target_properties(webfr PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

set(CMAKE_THREAD_PREFER_PTHREAD ON)

find_package(Threads REQUIRED)

SET(WEBFR_ADD_LINK_LIBS "")

if(MSVC OR MINGW)
        SET(WEBFR_ADD_LINK_LIBS wsock32 ws2_32)
elseif(NOT APPLE AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
        if(WEBFR_LINK_ATOMIC)
                SET(WEBFR_ADD_LINK_LIBS atomic)
        endif()
endif()

message("WEBFR_ADD_LINK_LIBS=${WEBFR_ADD_LINK_LIBS}")

target_link_libraries(webfr PUBLIC ${CMAKE_THREAD_LIBS_INIT}
        ${WEBFR_ADD_LINK_LIBS}
)

target_include_directories(webfr PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_library(webfr-test
        webfr-test/Checker.cpp
        webfr-test/Checker.hpp
        webfr-test/UnitTest.cpp
        webfr-test/UnitTest.hpp
        webfr-test/web/ClientServerTestRunner.hpp
)

set_target_properties(webfr-test PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(webfr-test PUBLIC webfr)

target_include_directories(webfr-test PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_dependencies(webfr-test webfr)

if(WEBFR_INSTALL)
    include("../cmake/module-install.cmake")
endif()