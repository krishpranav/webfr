#include "ApiClient.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"

namespace webfr { namespace web { namespace client {

std::shared_ptr<RequestExecutor::ConnectionHandle> ApiClient::getConnection() {
  return m_requestExecutor->getConnection();
}

webfr::async::CoroutineStarterForResult<const std::shared_ptr<RequestExecutor::ConnectionHandle>&> ApiClient::getConnectionAsync() {
  return m_requestExecutor->getConnectionAsync();
}

void ApiClient::invalidateConnection(const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle) {
  m_requestExecutor->invalidateConnection(connectionHandle);
}

ApiClient::StringTemplate ApiClient::parsePathTemplate(const webfr::String& name, const webfr::String& text) {

  std::vector<StringTemplate::Variable> variables;
  parser::Caret caret(text);

  while(caret.canContinue()) {

    if(caret.findChar('{')) {

      caret.inc();
      auto label = caret.putLabel();
      caret.findChar('}');
      label.end();

      StringTemplate::Variable var;
      var.posStart = label.getStartPosition() - 1;
      var.posEnd = label.getEndPosition();
      var.name = label.toString();

      variables.push_back(var);

    }

  }

  StringTemplate t(text, std::move(variables));
  auto extra = std::make_shared<PathTemplateExtra>();
  t.setExtraData(extra);

  extra->name = name;

  caret.setPosition(0);
  extra->hasQueryParams = caret.findChar('?');

  return t;

}

webfr::String ApiClient::formatPath(const StringTemplate& pathTemplate,
                                    const std::unordered_map<webfr::String, webfr::String>& pathParams,
                                    const std::unordered_map<webfr::String, webfr::String>& queryParams)
{

  data::stream::BufferOutputStream stream;
  stream << pathTemplate.format(pathParams);

  if(queryParams.size() > 0) {
    auto extra = std::static_pointer_cast<PathTemplateExtra>(pathTemplate.getExtraData());
    bool first = !extra->hasQueryParams;
    for(const auto& q : queryParams) {
      webfr::String value = q.second;
      if(value && value->getSize() > 0) {
        if (first) {
          stream.writeCharSimple('?');
          first = false;
        } else {
          stream.writeCharSimple('&');
        }
        stream << q.first << "=" << value;
      }
    }
  }

  return stream.toString();

}

std::shared_ptr<ApiClient::Response> ApiClient::executeRequest(const webfr::String& method,
                                                               const StringTemplate& pathTemplate,
                                                               const Headers& headers,
                                                               const std::unordered_map<webfr::String, webfr::String>& pathParams,
                                                               const std::unordered_map<webfr::String, webfr::String>& queryParams,
                                                               const std::shared_ptr<RequestExecutor::Body>& body,
                                                               const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle)
{

  return m_requestExecutor->execute(method,
                                    formatPath(pathTemplate, pathParams, queryParams),
                                    headers,
                                    body,
                                    connectionHandle);

}

webfr::async::CoroutineStarterForResult<const std::shared_ptr<ApiClient::Response>&>
ApiClient::executeRequestAsync(const webfr::String& method,
                               const StringTemplate& pathTemplate,
                               const Headers& headers,
                               const std::unordered_map<webfr::String, webfr::String>& pathParams,
                               const std::unordered_map<webfr::String, webfr::String>& queryParams,
                               const std::shared_ptr<RequestExecutor::Body>& body,
                               const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle)
{

  return m_requestExecutor->executeAsync(method,
                                         formatPath(pathTemplate, pathParams, queryParams),
                                         headers,
                                         body,
                                         connectionHandle);

}

}}}