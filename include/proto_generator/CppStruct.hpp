#pragma once

#include <proto_generator/CppField.hpp>

#include <vector>

namespace proto_generator
{

struct CppStruct
{
    std::vector<CppField> fields;

    std::string cpp_qualified_type;
    std::string proto_type;
};

struct CppVisitedStruct
{
    std::string qualified_name;
    std::string source_path;
};

} // namespace proto_generator

namespace jinja2
{

template <>
struct TypeReflection<proto_generator::CppStruct> : TypeReflected<proto_generator::CppStruct>
{
    using FieldAccessor = typename TypeReflected<proto_generator::CppStruct>::FieldAccessor;
    static auto& GetAccessors()
    {
        static std::unordered_map<std::string, FieldAccessor> accessors = {
            {"fields", [](const proto_generator::CppStruct& obj) { return Reflect(obj.fields); }},
            {"cpp_qualified_type", [](const proto_generator::CppStruct& obj) { return Reflect(obj.cpp_qualified_type); }},
            {"proto_type", [](const proto_generator::CppStruct& obj) { return Reflect(obj.proto_type); }},
        };

        return accessors;
    }
};

template <>
struct TypeReflection<proto_generator::CppVisitedStruct> : TypeReflected<proto_generator::CppVisitedStruct>
{
    using FieldAccessor = typename TypeReflected<proto_generator::CppVisitedStruct>::FieldAccessor;
    static auto& GetAccessors()
    {
        static std::unordered_map<std::string, FieldAccessor> accessors = {
            {"qualified_name", [](const proto_generator::CppVisitedStruct& obj) { return Reflect(obj.qualified_name); }},
            {"source_path", [](const proto_generator::CppVisitedStruct& obj) { return Reflect(obj.source_path); }},
        };

        return accessors;
    }
};

} // namespace jinja2
