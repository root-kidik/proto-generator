generate:
		clang++ -Xclang -load -Xclang ./build/lib/libproto_generator.so \
        -Xclang -plugin -Xclang proto_generator \
				-Xclang -plugin-arg-proto_generator -Xclang proto_out \
				-Xclang -plugin-arg-proto_generator -Xclang proto \
        -Xclang -plugin-arg-proto_generator -Xclang stdssr \
        -Xclang -plugin-arg-proto_generator -Xclang stdssr2 \
        $(shell find proto -name "*.hpp") -fsyntax-only && cd -
