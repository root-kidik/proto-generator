#pragma once

#include <jinja2cpp/reflected_value.h>
#include <string>

namespace proto_generator
{

struct CppField
{
    std::string name;
    std::string cpp_type;
    std::string proto_type;
    bool        is_trivial_proto_type{};
};

} // namespace proto_generator
//

namespace jinja2
{

template <>
struct TypeReflection<proto_generator::CppField> : TypeReflected<proto_generator::CppField>
{
    using FieldAccessor = typename TypeReflected<proto_generator::CppField>::FieldAccessor;
    static auto& GetAccessors()
    {
        static std::unordered_map<std::string, FieldAccessor> accessors = {
            {"name", [](const proto_generator::CppField& obj) { return Reflect(obj.name); }},
            {"cpp_type", [](const proto_generator::CppField& obj) { return Reflect(obj.cpp_type); }},
            {"proto_type", [](const proto_generator::CppField& obj) { return Reflect(obj.proto_type); }},
            {"is_trivial_proto_type",
             [](const proto_generator::CppField& obj) { return Reflect(obj.is_trivial_proto_type); }},
        };

        return accessors;
    }
};

} // namespace jinja2
