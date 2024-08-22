#pragma once

#include <proto_generator/CppTranslationUnit.hpp>
#include <proto_generator/ProtoGeneratorConsumer.hpp>

namespace proto_generator
{

class ProtoGeneratorAction : public clang::PluginASTAction
{
protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler_instance, llvm::StringRef) override;

    bool ParseArgs(const clang::CompilerInstance& compiler_instance, const std::vector<std::string>& args) override;

private:
    std::set<std::string> m_top_namespace_for_generating;
    std::string           m_output_directory;
    std::string           m_input_directory;
};

} // namespace proto_generator

static clang::FrontendPluginRegistry::Add<proto_generator::ProtoGeneratorAction> X("proto_generator",
                                                                                   "Generate .proto file from C++ "
                                                                                   "struct");
