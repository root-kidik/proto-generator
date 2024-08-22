#pragma once


#include <llvm/Support/raw_ostream.h>

#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Tooling/Tooling.h>

#include <proto_generator/CppTranslationUnit.hpp>

#include <jinja2cpp/template.h>

namespace proto_generator
{

class ProtoGeneratorConsumer : public clang::ASTConsumer
{
public:
    ProtoGeneratorConsumer(const clang::CompilerInstance& compiler_instance,
                           std::set<std::string>          top_namespace_for_generating,
                           std::string                    output_directory,
                           std::string                    input_directory);

    void HandleTranslationUnit(clang::ASTContext& context) override;

private:
    void Generate(const CppTranslationUnit& translation_unit,
                  const std::string&        file_extensions,
                  jinja2::Template&         jinja_template);

    std::string GetTranslationUnitPathWithoutInputDirectory();
    void        RemoveInputDirectoryPathFromVisitedStructs(std::vector<CppVisitedStruct>& cpp_visited_structs);
    void        CreateOutputDirectory(const std::string& translation_unit_source_path);

    const clang::CompilerInstance& m_compiler_instance;
    const clang::SourceManager&    m_source_manager;

    std::set<std::string> m_top_namespace_for_generating;
    std::string           m_output_directory;
    std::string           m_input_directory;

    jinja2::Template m_proto_file_template;
    jinja2::Template m_converter_file_template;
};

} // namespace proto_generator
