#include "PipeTest.hpp"
#include "webfr/network/virtual_/Pipe.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"
#include "webfr-test/Checker.hpp"
#include <iostream>
#include <thread>

namespace webfr { namespace test { namespace network { namespace virtual_ {
  
namespace {
  
  typedef webfr::network::virtual_::Pipe Pipe;
  
  const char* DATA_CHUNK = "<0123456789/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ>";
  const v_buff_size CHUNK_SIZE = std::strlen(DATA_CHUNK);

  class WriterTask : public webfr::base::Countable {
  private:
    std::shared_ptr<Pipe> m_pipe;
    v_int64 m_chunksToTransfer;
    v_buff_size m_position = 0;
    v_buff_size m_transferedBytes = 0;
  public:
    
    WriterTask(const std::shared_ptr<Pipe>& pipe, v_int64 chunksToTransfer)
      : m_pipe(pipe)
      , m_chunksToTransfer(chunksToTransfer)
    {}
    
    void run() {
      while (m_transferedBytes < CHUNK_SIZE * m_chunksToTransfer) {
        auto res = m_pipe->getWriter()->writeSimple(&DATA_CHUNK[m_position], CHUNK_SIZE - m_position);
        if(res > 0) {
          m_transferedBytes += res;
          m_position += res;
          if(m_position == CHUNK_SIZE) {
            m_position = 0;
          }
        }
      }
      WEBFR_LOGV("WriterTask", "sent %d bytes", m_transferedBytes);
    }
    
  };
  
  class ReaderTask : public webfr::base::Countable {
  private:
    std::shared_ptr<webfr::data::stream::ChunkedBuffer> m_buffer;
    std::shared_ptr<Pipe> m_pipe;
    v_int64 m_chunksToTransfer;
  public:
    
    ReaderTask(const std::shared_ptr<webfr::data::stream::ChunkedBuffer> &buffer,
               const std::shared_ptr<Pipe>& pipe,
               v_int64 chunksToTransfer)
      : m_buffer(buffer)
      , m_pipe(pipe)
      , m_chunksToTransfer(chunksToTransfer)
    {}
    
    void run() {
      v_char8 readBuffer[256];
      while (m_buffer->getSize() < CHUNK_SIZE * m_chunksToTransfer) {
        auto res = m_pipe->getReader()->readSimple(readBuffer, 256);
        if(res > 0) {
          m_buffer->writeSimple(readBuffer, res);
        }
      }
      WEBFR_LOGV("ReaderTask", "sent %d bytes", m_buffer->getSize());
    }
    
  };
  
  void runTransfer(const std::shared_ptr<Pipe>& pipe, v_int64 chunksToTransfer, bool writeNonBlock, bool readerNonBlock) {
    
    WEBFR_LOGV("transfer", "writer-nb: %d, reader-nb: %d", writeNonBlock, readerNonBlock);
    
    auto buffer = webfr::data::stream::ChunkedBuffer::createShared();
    
    {
      
      webfr::test::PerformanceChecker timer("timer");

      std::thread writerThread(&WriterTask::run, WriterTask(pipe, chunksToTransfer));
      std::thread readerThread(&ReaderTask::run, ReaderTask(buffer, pipe, chunksToTransfer));
    
      writerThread.join();
      readerThread.join();
      
    }
    
    WEBFR_ASSERT(buffer->getSize() == chunksToTransfer * CHUNK_SIZE);
    
    auto ruleBuffer = webfr::data::stream::ChunkedBuffer::createShared();
    for(v_int32 i = 0; i < chunksToTransfer; i ++) {
      ruleBuffer->writeSimple(DATA_CHUNK, CHUNK_SIZE);
    }
    
    auto str1 = buffer->toString();
    auto str2 = buffer->toString();
    
    WEBFR_ASSERT(str1 == str2);
    
  }
  
}
  
void PipeTest::onRun() {
  
  auto pipe = Pipe::createShared();

  v_int64 chunkCount = webfr::data::buffer::IOBuffer::BUFFER_SIZE * 10 / CHUNK_SIZE;
  
  runTransfer(pipe, chunkCount, false, false);
  runTransfer(pipe, chunkCount, true, false);
  runTransfer(pipe, chunkCount, false, true);
  runTransfer(pipe, chunkCount, true, true);

}
  
}}}}