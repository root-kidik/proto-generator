#pragma once

#include <llvm/Support/raw_ostream.h>

#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <clang/Tooling/Tooling.h>

#include <proto_generator/CppTranslationUnit.hpp>

#include <unordered_set>

namespace proto_generator
{

class ProtoGeneratorVisitor : public clang::RecursiveASTVisitor<ProtoGeneratorVisitor>
{
public:
    ProtoGeneratorVisitor(const clang::CompilerInstance& compiler_instance,
                          const clang::ASTContext&       context,
                          const std::set<std::string>&   top_namespace_for_generating);

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* declaration);

    std::vector<CppStruct>        GetAllCppStructs();
    std::vector<CppVisitedStruct> GetAllVisitedStructs();

private:
    void VisitStructDeclaration(clang::CXXRecordDecl* struct_declaration);

    bool IsThisDeclarationForGeneration(const clang::CXXRecordDecl* declaration);
    bool IsThisDeclarationFromMainFile(const clang::CXXRecordDecl* declaration);
    bool IsThisNamespaceDeclarationForGeneration(const clang::CXXRecordDecl* declaration);

    std::string GetCppFieldType(const clang::FieldDecl* declaration_field);
    std::string GetProtoFieldType(const clang::FieldDecl* field_declaration, const std::string& cpp_type);

    std::string FormatQualifiedNameToProtoType(const std::string& qualified_name);

    std::string GetFiledCppQualifiedType(const clang::FieldDecl* field_del);

    const clang::CompilerInstance& m_compiler_instance;
    const clang::ASTContext&       m_context;
    const clang::SourceManager&    m_source_manager;

    const std::set<std::string>& m_top_namespace_for_generating;

    std::vector<CppVisitedStruct> m_visited_structs;
    std::vector<CppStruct>        m_cpp_structs;

    std::unordered_set<std::string> m_processed_structs;
};

} // namespace proto_generator
