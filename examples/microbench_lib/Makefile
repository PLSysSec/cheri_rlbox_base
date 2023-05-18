.PHONY: clean dir all_mswasm all_wasm2c

include ../Makefile.inc

BUILDDIR ?= build 
OUTDIR ?= out

dir:
	mkdir -p build 
	mkdir -p $(OUTDIR)

################### Native builds ####################

build/my_lib.wasm: lib.c lib.h dir
	$(WASI_SDK_ROOT)/bin/clang --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o build/wasm2c_sandbox_wrapper.o
	$(WASI_SDK_ROOT)/bin/clang++ --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot/ lib.c build/wasm2c_sandbox_wrapper.o -o $@  $(WASI_SDK_FLAGS)

build/my_lib.wasm.c: build/my_lib.wasm
	$(WASM2C_ROOT)/bin/wasm2c $< -o my_lib.wasm.c 
	mv my_lib.wasm.c build
	mv my_lib.wasm.h build
	
$(OUTDIR)/my_lib_wasm2c.so: build/my_lib.wasm.c 
	$(CC) -I$(WASM2C_ROOT) -shared -fPIC -O3 $< -o $@  $(WASM2C_ADDINS)

build/my_lib.o: lib.c lib.h dir
	$(CC) lib.c -c -o $@

$(OUTDIR)/my_lib_native.so: lib.c lib.h dir
	$(CXX) --shared -fPIC lib.c -o $@


$(OUTDIR)/microbenchmarks_wasm2c: $(OUTDIR)/my_lib_wasm2c.so microbenchmarks.cpp
	$(CXX) -std=c++17 -DWASM2C_SANDBOX microbenchmarks.cpp -ferror-limit=100 -o $@ $(RLBOX_INCLUDES) $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

$(OUTDIR)/microbenchmarks_noop: build/my_lib.o microbenchmarks.cpp
	$(CXX) -std=c++17 -DNOOP_SANDBOX microbenchmarks.cpp $< -o $@ $(RLBOX_INCLUDES) $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

all_wasm2c: $(OUTDIR)/microbenchmarks_noop $(OUTDIR)/microbenchmarks_wasm2c

#################### Cross-compiled Cheri builds ##############################

# Compile object in purecap mode for cheri-noop
build/my_lib_cheri.o: lib.c lib.h dir
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) lib.c -c -o $@

# Compile dynamic library in purecap mode for cheri-dylib
$(OUTDIR)/my_lib_cheri.so: lib.c lib.h dir
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS)  --shared -fPIC lib.c -o $@

# 1) Morello noop pure => pure
$(OUTDIR)/microbenchmarks_cheri_pure_noop: build/my_lib_cheri.o microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_NOOP_SANDBOX $^ -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)

# 2) Morello dylib pure => pure
$(OUTDIR)/microbenchmarks_cheri_pure_dylib: $(OUTDIR)/my_lib_cheri.so microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_DYLIB_SANDBOX microbenchmarks.cpp -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)

all_cheri: $(OUTDIR)/microbenchmarks_cheri_pure_noop 



#########################Cross-compiled Cheri MSWasm build ##########################

build/my_lib.mswasm: lib.c lib.h dir
	$(C_TO_MSWASM) lib.c -c -o $@  

build/my_lib.mswasm.c: build/my_lib.mswasm
	$(RWASM) $(RWASM_FLAGS) build/my_lib.mswasm  build/my_lib.mswasm

$(OUTDIR)/my_lib_mswasm_cheri.so: build/my_lib.mswasm.c 
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -lm --shared -fPIC $< -o $@

# Morello mswasm pure => pure
$(OUTDIR)/microbenchmarks_cheri_mswasm: $(OUTDIR)/my_lib_mswasm_cheri.so microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_MSWASM_SANDBOX microbenchmarks.cpp -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)

all_mswasm: $(OUTDIR)/microbenchmarks_cheri_mswasm


clean:
	-rm -rf build 
	-rm -rf $(OUTDIR) 


