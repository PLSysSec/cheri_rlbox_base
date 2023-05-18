
# Tool definitions
CC=clang 
CXX=clang++


# Tool-root definitions
# TODO: prebuilt morello-sdk binaries?
MORELLO_SDK_ROOT:=$(HOME)/cheri/output/morello-sdk
RLBOX_ROOT:=../../cheri_rlbox

# TODO: change these paths and add them formally to repo
RWASM_ROOT?=../../tools/rWasm
MSWASM_SDK_ROOT?=../../tools/mswasm-wasi-sdk


# Morello cross-compilation definitions
MORELLO_CC=$(MORELLO_SDK_ROOT)/bin/clang
MORELLO_CXX=$(MORELLO_SDK_ROOT)/bin/clang++
MORELLO_SYSROOT=$(MORELLO_SDK_ROOT)/sysroot-morello-purecap
# Currently compiles for cheribsd, might need extra work for linux
MORELLO_PURE_FLAGS=--target=aarch64-unknown-freebsd13 -march=morello+c64 -mabi=purecap --sysroot=$(MORELLO_SYSROOT) 
MORELLO_HYBRID_FLAGS=--target=aarch64-unknown-freebsd13 -march=morello --sysroot=$(MORELLO_SYSROOT) 
MORELLO_CFLAGS= -Wall -Wcheri -g 

# Rlbox definitions
RLBOX_INCLUDES=-I $(RLBOX_ROOT)/build/_deps/rlbox-src/code/include -I $(RLBOX_ROOT)/include
RLBOX_FLAGS= -pthread -ldl

# Wasm definitions
WASI_SDK_ROOT:=$(RLBOX_ROOT)/build/_deps/wasiclang-src/
WASM2C_ROOT=./$(RLBOX_ROOT)/build/_deps/mod_wasm2c-src
WASM2C_ADDINS=$(WASM2C_ROOT)/wasm2c/wasm-rt-impl.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-unix.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-win.c $(WASM2C_ROOT)/wasm2c/wasm-rt-wasi.c -I$(WASM2C_ROOT)/wasm2c/
WASI_SDK_FLAGS=-Wl,--export-all -Wl,--growable-table -Wl,--no-entry
# WASM2C_INCLUDES= -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/


# mswasm paths
RWASM=$(RWASM_ROOT)/target/debug/rwasm
MSWASM_INSTALL=$(MSWASM_SDK_ROOT)/build/install/opt/wasi-sdk/
MSWASM_CC:=$(MSWASM_INSTALL)/bin/clang
MSWASM_CXX:=$(MSWASM_INSTALL)/bin/clang++
MSWASM_SYSROOT:=$(MSWASM_INSTALL)/share/wasi-sysroot

RWASM_FLAGS=--wasi --library --ms-wasm --cheri

C_TO_MSWASM=$(MSWASM_CC) --target=wasm32-wasi --sysroot $(MSWASM_SYSROOT)