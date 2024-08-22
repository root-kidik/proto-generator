#include <proto_generator/ProtoGeneratorConsumer.hpp>
#include <proto_generator/ProtoGeneratorVisitor.hpp>

#include <boost/algorithm/string/replace.hpp>
#include <filesystem>
#include <fstream>
#include <jinja2cpp/template.h>

#include "jinja2cpp/reflected_value.h"

namespace proto_generator
{

ProtoGeneratorConsumer::ProtoGeneratorConsumer(const clang::CompilerInstance& compiler_instance,
                                               std::set<std::string>          top_namespace_for_generating,
                                               std::string                    output_directory,
                                               std::string                    input_directory) :
m_compiler_instance{compiler_instance},
m_source_manager{compiler_instance.getSourceManager()},
m_top_namespace_for_generating{std::move(top_namespace_for_generating)},
m_output_directory{std::move(output_directory)},
m_input_directory{std::move(input_directory)}
{
    m_proto_file_template.LoadFromFile("templates/proto.jinja2");
    m_converter_file_template.LoadFromFile("templates/converter.jinja2");
}

void ProtoGeneratorConsumer::HandleTranslationUnit(clang::ASTContext& context)
{
    ProtoGeneratorVisitor visitor{m_compiler_instance, context, m_top_namespace_for_generating};

    visitor.TraverseDecl(context.getTranslationUnitDecl());

    CppTranslationUnit translation_unit;
    translation_unit.structs         = visitor.GetAllCppStructs();
    translation_unit.visited_structs = visitor.GetAllVisitedStructs();
    translation_unit.source_path     = GetTranslationUnitPathWithoutInputDirectory();

    RemoveInputDirectoryPathFromVisitedStructs(translation_unit.visited_structs);

    CreateOutputDirectory(translation_unit.source_path);

    Generate(translation_unit, ".proto", m_proto_file_template);
    Generate(translation_unit, ".pb.converter.inl", m_converter_file_template);
}

void ProtoGeneratorConsumer::Generate(const CppTranslationUnit& translation_unit,
                                      const std::string&        file_extensions,
                                      jinja2::Template&         jinja_template)
{
    std::string generated_filename = boost::replace_last_copy(translation_unit.source_path, ".hpp", file_extensions);
    boost::replace_last(generated_filename, ".h", file_extensions);

    std::filesystem::path generated_file_path = m_output_directory + m_input_directory + generated_filename;
    std::ofstream         generated_file{generated_file_path};

    jinja2::ValuesMap bindings;
    bindings["translation_unit"] = jinja2::Reflect(translation_unit);
    generated_file << jinja_template.RenderAsString(bindings).value();
}

std::string ProtoGeneratorConsumer::GetTranslationUnitPathWithoutInputDirectory()
{
    return boost::replace_first_copy(m_source_manager.getFileEntryRefForID(m_source_manager.getMainFileID())->getName().str(),
                                     m_input_directory,
                                     "");
}

void ProtoGeneratorConsumer::RemoveInputDirectoryPathFromVisitedStructs(std::vector<CppVisitedStruct>& cpp_visited_structs)
{
    for (auto& [qualified_name, source_path] : cpp_visited_structs)
        boost::replace_first(source_path, m_input_directory, "");
}

void ProtoGeneratorConsumer::CreateOutputDirectory(const std::string& translation_unit_source_path)
{
    size_t      pos                   = translation_unit_source_path.find_last_of("/\\");
    std::string full_output_directory = m_output_directory + m_input_directory +
                                        translation_unit_source_path.substr(0, pos + 1);

    if (!std::filesystem::exists(full_output_directory))
        std::filesystem::create_directories(full_output_directory);
}

} // namespace proto_generator
