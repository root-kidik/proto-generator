# proto-generator

```
sudo apt install llvm-dev libclang-dev libclang-cpp-dev
```

```
clang++ -Xclang -load -Xclang ./build/lib/libproto_generator.so -Xclang -plugin -Xclang proto_generator -Xclang stdssr Xclang stdssr2 Hello.hpp -fsyntax-only
```

```
clang++ -Xclang -load -Xclang ./build/lib/libproto_generator.so \
        -Xclang -plugin -Xclang proto_generator \
        -Xclang -plugin-arg-proto_generator -Xclang stdssr \
        -Xclang -plugin-arg-proto_generator -Xclang stdssr2 \
        Hello.hpp -fsyntax-only

```

### TODO:
Обработать `using`