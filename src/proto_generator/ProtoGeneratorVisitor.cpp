#include <proto_generator/ProtoGeneratorVisitor.hpp>

#include <boost/algorithm/string/replace.hpp>
#include <unordered_map>

namespace
{

const std::unordered_map<std::string, std::string>
    kFundamentalTypeMappings{{"signed char", "int32"},
                             {"unsigned char", "int32"},
                             {"short", "int32"},
                             {"short int", "int32"},
                             {"signed short", "int32"},
                             {"signed short int", "int32"},
                             {"unsigned short", "uint32"},
                             {"unsigned short int", "uint32"},
                             {"int", "int32"},
                             {"signed", "int32"},
                             {"signed int", "int32"},
                             {"unsigned", "uint32"},
                             {"unsigned int", "uint32"},
                             {"long", "int32"},
                             {"long int", "int32"},
                             {"signed long", "int32"},
                             {"signed long int", "int32"},
                             {"unsigned long", "uint32"},
                             {"unsigned long int", "uint32"},
                             {"long long", "int64"},
                             {"long long int", "int64"},
                             {"signed long long", "int64"},
                             {"signed long long int", "int64"},
                             {"unsigned long long", "uint64"},
                             {"unsigned long long int", "uint64"},
                             {"std::string", "string"}};

const std::unordered_set<std::string> kProtoFundamentalTypes = {"int32", "uint32", "int64", "uint64", "string"};
} // namespace

namespace proto_generator
{

ProtoGeneratorVisitor::ProtoGeneratorVisitor(const clang::CompilerInstance& compiler_instance,
                                             const clang::ASTContext&       context,
                                             const std::set<std::string>&   top_namespace_for_generating) :
m_compiler_instance{compiler_instance},
m_context{context},
m_source_manager{compiler_instance.getSourceManager()},
m_top_namespace_for_generating{top_namespace_for_generating}
{
}

bool ProtoGeneratorVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* declaration)
{
    if (!IsThisDeclarationForGeneration(declaration))
        return true;

    VisitStructDeclaration(declaration);

    return true;
}

void ProtoGeneratorVisitor::VisitStructDeclaration(clang::CXXRecordDecl* struct_declaration)
{
    auto struct_qualified_name = struct_declaration->getQualifiedNameAsString();
    m_processed_structs.emplace(struct_qualified_name);

    CppStruct cpp_struct;
    cpp_struct.cpp_qualified_type = struct_qualified_name;
    cpp_struct.proto_type         = FormatQualifiedNameToProtoType(struct_qualified_name);

    for (const auto* declaration_field : struct_declaration->fields())
    {
        CppField cpp_field;

        cpp_field.name       = declaration_field->getName();
        cpp_field.cpp_type   = GetCppFieldType(declaration_field);
        cpp_field.proto_type = GetProtoFieldType(declaration_field, cpp_field.cpp_type);
        if (kProtoFundamentalTypes.contains(cpp_field.proto_type))
            cpp_field.is_trivial_proto_type = true;

        cpp_struct.fields.push_back(std::move(cpp_field));
    }

    m_cpp_structs.push_back(cpp_struct);
}

std::vector<CppStruct> ProtoGeneratorVisitor::GetAllCppStructs()
{
    return std::move(m_cpp_structs);
}

std::vector<CppVisitedStruct> ProtoGeneratorVisitor::GetAllVisitedStructs()
{
    return std::move(m_visited_structs);
}

bool ProtoGeneratorVisitor::IsThisDeclarationForGeneration(const clang::CXXRecordDecl* declaration)
{
    if (!IsThisDeclarationFromMainFile(declaration))
        return false;

    if (!declaration->isThisDeclarationADefinition())
        return false;

    if (!IsThisNamespaceDeclarationForGeneration(declaration))
        return false;

    if (!declaration->isStruct())
        return false;

    return true;
}

bool ProtoGeneratorVisitor::IsThisDeclarationFromMainFile(const clang::CXXRecordDecl* declaration)
{
    return m_source_manager.isInMainFile(declaration->getLocation());
}

bool ProtoGeneratorVisitor::IsThisNamespaceDeclarationForGeneration(const clang::CXXRecordDecl* declaration)
{
    auto* declaration_context = declaration->getDeclContext();

    std::string top_namespace;
    while (declaration_context && !llvm::isa<clang::TranslationUnitDecl>(declaration_context))
    {
        if (const auto* namespace_declaration = llvm::dyn_cast<clang::NamespaceDecl>(declaration_context))
            top_namespace = namespace_declaration->getNameAsString();

        declaration_context = declaration_context->getParent();
    }

    return m_top_namespace_for_generating.contains(top_namespace);
}

std::string ProtoGeneratorVisitor::GetCppFieldType(const clang::FieldDecl* declaration_field)
{
    if (auto canonical_type = declaration_field->getType().getCanonicalType(); canonical_type->isBuiltinType())
        return canonical_type.getAsString();

    return declaration_field->getType().getAsString();
}

std::string ProtoGeneratorVisitor::GetProtoFieldType(const clang::FieldDecl* field_declaration, const std::string& cpp_type)
{
    if (auto it = kFundamentalTypeMappings.find(cpp_type); it != kFundamentalTypeMappings.end())
        return it->second;

    return FormatQualifiedNameToProtoType(GetFiledCppQualifiedType(field_declaration));
}

std::string ProtoGeneratorVisitor::FormatQualifiedNameToProtoType(const std::string& qualified_name)
{
    return boost::replace_all_copy(qualified_name, "::", "_");
}

std::string ProtoGeneratorVisitor::GetFiledCppQualifiedType(const clang::FieldDecl* field_del)
{
    const clang::RecordType* record_type = field_del->getType().getTypePtrOrNull()->getAs<clang::RecordType>();
    assert(record_type);

    auto record_declaration   = llvm::dyn_cast<clang::CXXRecordDecl>(record_type->getDecl());
    auto field_qualified_type = record_declaration->getQualifiedNameAsString();

    if (!m_processed_structs.contains(field_qualified_type))
        m_visited_structs.emplace_back(field_qualified_type,
                                       m_source_manager.getPresumedLoc(record_declaration->getLocation()).getFilename());
    else
        m_processed_structs.emplace(field_qualified_type);

    return field_qualified_type;
}

} // namespace proto_generator
