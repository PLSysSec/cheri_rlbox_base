.PHONY: bootstrap clean

RLBOX_ROOT:=./cheri_rlbox
WASI_SDK_ROOT:=$(RLBOX_ROOT)/build/_deps/wasiclang-src/
WASM2C_ROOT=./$(RLBOX_ROOT)/build/_deps/mod_wasm2c-src

WASI_SDK_FLAGS=-Wl,--export-all -Wl,--growable-table -Wl,--no-entry

CC_HYBRID=clang -march=morello
CXX_HYBRID=clang++ -march=morello
CC_PURE=cc
CXX_PURE=c++


bootstrap:
	git submodule update --init --recursive
	cd tools/mswasm-wasi-sdk && make && ln -s build/install/opt/wasi-sdk/lib/clang/11.0.0/lib/ build/install/opt/wasi-sdk/lib/clang/lib/
	cd tools/rWasm && cargo build
	cd cheri_rlbox && cmake -S . -B ./build && cmake --build ./build --parallel && cmake --build ./build --target all
	cd tools/cheribuild && ./cheribuild.py sdk-morello-purecap


# Bootstrap sandboxing and compiler toolchain for compiling on non-CHERI system
# bootstrap_host:
# 	git submodule update --init --recursive
# 	cd cheri_rlbox && cmake -S . -B ./build && cmake --build ./build --parallel && cmake --build ./build --target all

# Can't build all of rlbox on Cheri guest, but we still need headers which rlbox pulls in.
# So we just fake it
# bootstrap_guest:
# 	git submodule update --init --recursive
# 	mkdir -p $(RLBOX_ROOT)/build/_deps
# 	cd $(RLBOX_ROOT)/build/_deps && git clone https://github.com/PLSysSec/wasm2c_sandbox_compiler mod_wasm2c-src
# 	cd $(RLBOX_ROOT)/build/_deps && git clone https://github.com/PLSysSec/rlbox_api_cpp17.git rlbox-src