#include "InterfaceTest.hpp"
#include "webfr/network/virtual_/Interface.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"
#include <thread>
#include <list>

namespace webfr { namespace test { namespace network { namespace virtual_ {
  
namespace {
  
  typedef webfr::network::virtual_::Interface Interface;
  typedef webfr::network::virtual_::Socket Socket;
  typedef std::list<std::thread> ThreadList;
  
  class ClientTask : public webfr::base::Countable {
  private:
    std::shared_ptr<Interface> m_interface;
    webfr::String m_dataSample;
  public:
    
    ClientTask(const std::shared_ptr<Interface>& interface,
               const webfr::String& dataSample)
      : m_interface(interface)
      , m_dataSample(dataSample)
    {}
    
    void run() {
      auto submission = m_interface->connect();
      auto socket = submission->getSocket();
      
      auto res = socket->writeExactSizeDataSimple(m_dataSample->getData(), m_dataSample->getSize());
      WEBFR_ASSERT(res == m_dataSample->getSize());
      
      v_char8 buffer[100];
      webfr::data::stream::ChunkedBuffer stream;
      res = webfr::data::stream::transfer(socket.get(), &stream, 2, buffer, 100);
      
      WEBFR_ASSERT(res == 2);
      WEBFR_ASSERT(stream.getSize() == res);
      WEBFR_ASSERT(stream.toString() == "OK");
      
      //WEBFR_LOGV("client", "finished - OK");
      
    }
    
  };
  
  class ServerTask : public webfr::base::Countable {
  private:
    std::shared_ptr<Socket> m_socket;
    webfr::String m_dataSample;
  public:
    
    ServerTask(const std::shared_ptr<Socket>& socket,
               const webfr::String& dataSample)
      : m_socket(socket)
      , m_dataSample(dataSample)
    {}
    
    void run() {
      v_char8 buffer[100];
      webfr::data::stream::ChunkedBuffer stream;
      auto res = webfr::data::stream::transfer(m_socket.get(), &stream, m_dataSample->getSize(), buffer, 100);
      
      WEBFR_ASSERT(res == m_dataSample->getSize());
      WEBFR_ASSERT(stream.getSize() == res);
      WEBFR_ASSERT(stream.toString() == m_dataSample);
      
      res = m_socket->writeExactSizeDataSimple("OK", 2);
      
      WEBFR_ASSERT(res == 2);
    }
    
  };
  
  class Server : public webfr::base::Countable {
  private:
    std::shared_ptr<Interface> m_interface;
    webfr::String m_dataSample;
    v_int32 m_numTasks;
  public:
    
    Server(const std::shared_ptr<Interface>& interface,
           const webfr::String& dataSample,
           v_int32 numTasks)
      : m_interface(interface)
      , m_dataSample(dataSample)
      , m_numTasks(numTasks)
    {}
    
    void run() {
      ThreadList threadList;
      for(v_int32 i = 0; i < m_numTasks; i++) {
        auto socket = m_interface->accept();
        threadList.push_back(std::thread(&ServerTask::run, ServerTask(socket, m_dataSample)));
      }
      for(auto& thread : threadList) {
        thread.join();
      }
    }
    
  };
  
}
  
void InterfaceTest::onRun() {
  
  webfr::String dataSample = "1234567890-=][poiuytrewqasdfghjkl;'/.,mnbvcxzzxcvbnm,./';lkjhgfdsaqwertyuiop][=-0987654321";
  
  auto interface = Interface::obtainShared("virtualhost");
  auto bindLock = interface->bind();

  v_int32 numTasks = 100;
  
  ThreadList threadList;
  
  std::thread server(&Server::run, Server(interface, dataSample, numTasks));
  
  for(v_int32 i = 0; i < numTasks; i++) {
    threadList.push_back(std::thread(&ClientTask::run, ClientTask(interface, dataSample)));
  }

  for(auto& thread : threadList) {
    thread.join();
  }
  
  server.join();

}
  
}}}}