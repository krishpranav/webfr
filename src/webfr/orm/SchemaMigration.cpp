#include "SchemaMigration.hpp"
#include <algorithm>

namespace webfr { namespace orm {

SchemaMigration::SchemaMigration(const base::ObjectHandle<Executor>& executor, const webfr::String& suffix)
  : m_executor(executor)
  , m_suffix(suffix)
{}

void SchemaMigration::addText(v_int64 version, const webfr::String& script) {
  m_scripts.push_back({version, SOURCE_TEXT, script});
}

void SchemaMigration::addFile(v_int64 version, const webfr::String& filename) {
  m_scripts.push_back({version, SOURCE_FILE, filename});
}

void SchemaMigration::migrate() {

  if(!m_executor) {
    throw std::runtime_error("[webfr::orm::SchemaMigration::migrate()]: Error. Executor is null.");
  }

  auto connection = m_executor->getConnection();

  if(!connection) {
    throw std::runtime_error("[webfr::orm::SchemaMigration::migrate()]: Error. Can't connect to Database.");
  }

  v_int64 currVersion = m_executor->getSchemaVersion(m_suffix, connection);

  std::sort(m_scripts.begin(), m_scripts.end(), [](const Source& a, const Source& b) {
    return a.version < b.version;
  });

  for(auto& source : m_scripts) {

    if(source.version > currVersion) {

      webfr::String script;

      switch (source.type) {

        case SOURCE_TEXT:
          script = source.param;
          break;

        case SOURCE_FILE:
          script = base::StrBuffer::loadFromFile(source.param->c_str());
          break;

        default:
          throw std::runtime_error("[webfr::orm::SchemaMigration::migrate()]: Error. Unknown source type.");

      }

      m_executor->migrateSchema(script, source.version, m_suffix, connection);

    }

  }

}

}}