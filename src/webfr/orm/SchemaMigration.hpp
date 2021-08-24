#ifndef webfr_orm_SchemaMigration_hpp
#define webfr_orm_SchemaMigration_hpp

#include "Executor.hpp"

namespace webfr { namespace orm {

class SchemaMigration {
private:

  static constexpr v_int32 SOURCE_TEXT = 0;
  static constexpr v_int32 SOURCE_FILE = 1;

  struct Source {
    v_int64 version;
    v_int32 type;
    webfr::String param;
  };

private:
  base::ObjectHandle<Executor> m_executor;
  webfr::String m_suffix;
  std::vector<Source> m_scripts;
public:

  SchemaMigration(const base::ObjectHandle<Executor>& executor, const webfr::String& suffix = nullptr);

  virtual ~SchemaMigration() = default;

  void addText(v_int64 version, const webfr::String& script);

  void addFile(v_int64 version, const webfr::String& filename);

  void migrate();

};

}}

#endif 