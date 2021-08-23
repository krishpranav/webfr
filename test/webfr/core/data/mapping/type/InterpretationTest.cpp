// includes
#include "InterpretationTest.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/data/mapping/TypeResolver.hpp"
#include "webfr/core/Types.hpp"
#include "webfr/core/macro/codegen.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

namespace {

#include WEBFR_CODEGEN_BEGIN(DTO)

struct VPoint {
  v_int32 x;
  v_int32 y;
  v_int32 z;
};

struct VLine {
  VPoint p1;
  VPoint p2;
};

namespace __class {
  class PointClass;
  class LineClass;
}

typedef webfr::data::mapping::type::Primitive<VPoint, __class::PointClass> Point;
typedef webfr::data::mapping::type::Primitive<VLine, __class::LineClass> Line;

namespace __class {

  class PointClass {
  private:

    class PointDto : public webfr::DTO {

      DTO_INIT(PointDto, DTO)

      DTO_FIELD(Int32, x);
      DTO_FIELD(Int32, y);
      DTO_FIELD(Int32, z);

    };

    class Inter : public webfr::Type::Interpretation<Point, webfr::Object<PointDto>>  {
    public:

      webfr::Object<PointDto> interpret(const Point& value) const override {
        WEBFR_LOGD("Point::Interpretation", "interpret");
        auto dto = PointDto::createShared();
        dto->x = value->x;
        dto->y = value->y;
        dto->z = value->z;
        return dto;
      }

      Point reproduce(const webfr::Object<PointDto>& value) const override {
        WEBFR_LOGD("Point::Interpretation", "reproduce");
        return Point({value->x, value->y, value->z});
      }

    };

  public:

    static const webfr::ClassId CLASS_ID;

    static webfr::Type* getType(){
      static Type type(
        CLASS_ID, nullptr, nullptr,
        {
          {"test", new Inter()}
        }
      );
      return &type;
    }

  };

  const webfr::ClassId PointClass::CLASS_ID("test::Point");

  class LineClass {
  private:

    class LineDto : public webfr::DTO {

      DTO_INIT(LineDto, DTO)

      DTO_FIELD(Point, p1);
      DTO_FIELD(Point, p2);

    };

    class Inter : public webfr::Type::Interpretation<Line, webfr::Object<LineDto>>  {
    public:

      webfr::Object<LineDto> interpret(const Line& value) const override {
        WEBFR_LOGD("Line::Interpretation", "interpret");
        auto dto = LineDto::createShared();
        dto->p1 = {value->p1.x, value->p1.y, value->p1.z};
        dto->p2 = {value->p2.x, value->p2.y, value->p2.z};
        return dto;
      }

      Line reproduce(const webfr::Object<LineDto>& value) const override {
        WEBFR_LOGD("Line::Interpretation", "reproduce");
        return Line({{value->p1->x, value->p1->y, value->p1->z},
                     {value->p2->x, value->p2->y, value->p2->z}});
      }

    };

  public:

    static const webfr::ClassId CLASS_ID;

    static webfr::Type* getType(){
      static Type type(
        CLASS_ID, nullptr, nullptr,
        {
          {"test", new Inter()}
        }
      );
      return &type;
    }

  };

  const webfr::ClassId LineClass::CLASS_ID("test::Line");

}

#include WEBFR_CODEGEN_END(DTO)

}

void InterpretationTest::onRun() {

  webfr::parser::json::mapping::ObjectMapper mapper;

  {
    auto config = mapper.getSerializer()->getConfig();
    config->enabledInterpretations = {"test"};
    config->useBeautifier = false;
  }

  {
    auto config = mapper.getDeserializer()->getConfig();
    config->enabledInterpretations = {"test"};
  }

  Point p1 ({1, 2, 3});
  Point p2 ({11, 12, 13});

  Line l ({p1, p2});

  auto json1 = mapper.writeToString(l);

  WEBFR_LOGD(TAG, "json1='%s'", json1->c_str());

  auto rl = mapper.readFromString<Line>(json1);

  auto json2 = mapper.writeToString(rl);

  WEBFR_LOGD(TAG, "json2='%s'", json2->c_str());

  WEBFR_ASSERT(json1 == json2);

  webfr::data::mapping::TypeResolver::Cache cache;

  {
    webfr::data::mapping::TypeResolver tr;
    tr.setEnabledInterpretations({"test"});

    //webfr::data::mapping::TypeResolver::Cache cache;

    auto v = tr.resolveObjectPropertyValue(l, {"p1", "x"}, cache);
    WEBFR_ASSERT(v);
    WEBFR_ASSERT(v.valueType == webfr::Int32::Class::getType());
    WEBFR_ASSERT(v.staticCast<webfr::Int32>() == 1);
  }

  {
    webfr::data::mapping::TypeResolver tr;
    tr.setEnabledInterpretations({"test"});

    //webfr::data::mapping::TypeResolver::Cache cache;

    auto v = tr.resolveObjectPropertyValue(l, {"p1", "y"}, cache);
    WEBFR_ASSERT(v);
    WEBFR_ASSERT(v.valueType == webfr::Int32::Class::getType());
    WEBFR_ASSERT(v.staticCast<webfr::Int32>() == 2);
  }

  {
    webfr::data::mapping::TypeResolver tr;
    tr.setEnabledInterpretations({"test"});

    //webfr::data::mapping::TypeResolver::Cache cache;

    auto v = tr.resolveObjectPropertyValue(l, {"p1", "z"}, cache);
    WEBFR_ASSERT(v);
    WEBFR_ASSERT(v.valueType == webfr::Int32::Class::getType());
    WEBFR_ASSERT(v.staticCast<webfr::Int32>() == 3);
  }

}

}}}}}}