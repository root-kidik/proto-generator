#include <proto_generator/ProtoGeneratorAction.hpp>

namespace proto_generator
{

std::unique_ptr<clang::ASTConsumer> ProtoGeneratorAction::CreateASTConsumer(clang::CompilerInstance& compiler_instance,
                                                                            llvm::StringRef)
{
    return std::make_unique<ProtoGeneratorConsumer>(compiler_instance,
                                                    std::move(m_top_namespace_for_generating),
                                                    std::move(m_output_directory),
                                                    std::move(m_input_directory));
}

bool ProtoGeneratorAction::ParseArgs(const clang::CompilerInstance& compiler_instance, const std::vector<std::string>& args)
{
    m_output_directory = args[0];
    if (m_output_directory.back() != '/')
        m_output_directory.push_back('/');

    m_input_directory = args[1];
    if (m_input_directory.back() != '/')
        m_input_directory.push_back('/');

    std::copy(args.begin() + 2,
              args.end(),
              std::inserter(m_top_namespace_for_generating, m_top_namespace_for_generating.end()));

    return true;
}

} // namespace proto_generator
