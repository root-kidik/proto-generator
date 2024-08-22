set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(SRC
    ${INCROOT}/${PROJECT_NAME}/ProtoGeneratorAction.hpp
    ${SRCROOT}/${PROJECT_NAME}/ProtoGeneratorAction.cpp
    ${INCROOT}/${PROJECT_NAME}/ProtoGeneratorConsumer.hpp
    ${SRCROOT}/${PROJECT_NAME}/ProtoGeneratorConsumer.cpp
    ${INCROOT}/${PROJECT_NAME}/ProtoGeneratorVisitor.hpp
    ${SRCROOT}/${PROJECT_NAME}/ProtoGeneratorVisitor.cpp
    ${INCROOT}/${PROJECT_NAME}/CppField.hpp
    ${INCROOT}/${PROJECT_NAME}/CppStruct.hpp
    ${INCROOT}/${PROJECT_NAME}/CppTranslationUnit.hpp
)

add_library(${PROJECT_NAME} SHARED ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCROOT} ${LLVM_INCLUDE_DIR} ${CLANG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE clangAST clangBasic clangFrontend clangSerialization clangTooling Boost::headers jinja2cpp)
