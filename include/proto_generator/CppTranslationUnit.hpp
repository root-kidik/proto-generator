#pragma once

#include <proto_generator/CppStruct.hpp>

#include <vector>

namespace proto_generator
{

struct CppTranslationUnit
{
    std::vector<CppStruct>        structs;
    std::vector<CppVisitedStruct> visited_structs;
    std::string                   source_path;
};

} // namespace proto_generator

namespace jinja2
{

template <>
struct TypeReflection<proto_generator::CppTranslationUnit> : TypeReflected<proto_generator::CppTranslationUnit>
{
    using FieldAccessor = typename TypeReflected<proto_generator::CppTranslationUnit>::FieldAccessor;
    static auto& GetAccessors()
    {
        static std::unordered_map<std::string, FieldAccessor> accessors = {
            {"structs", [](const proto_generator::CppTranslationUnit& obj) { return Reflect(obj.structs); }},
            {"visited_structs",
             [](const proto_generator::CppTranslationUnit& obj) { return Reflect(obj.visited_structs); }},
            {"source_path", [](const proto_generator::CppTranslationUnit& obj) { return Reflect(obj.source_path); }},

        };

        return accessors;
    }
};

} // namespace jinja2
