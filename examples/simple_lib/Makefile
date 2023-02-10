.PHONY: clean dir all_mswasm

include ../Makefile.inc


BUILDDIR ?= build 
OUTDIR ?= out

dir:
	mkdir -p build 
	mkdir -p $(OUTDIR)

########################### Native Builds ######################################
# my_lib.wasm: lib.c lib.h
# 	$(WASI_SDK_ROOT)/bin/clang --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o wasm2c_sandbox_wrapper.o
# 	$(WASI_SDK_ROOT)/bin/clang++ --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot/ lib.c wasm2c_sandbox_wrapper.o -o my_lib.wasm  $(WASI_SDK_FLAGS)


# my_lib.wasm.c: my_lib.wasm
# 	$(WASM2C_ROOT)/bin/wasm2c my_lib.wasm -o my_lib.wasm.c
	
# my_lib.so: my_lib.wasm.c 
# 	$(CC_PURE) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ADDINS)


# my_lib_cheri.so: 
# 	$(CC_PURE) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so   $(WASM2C_ADDINS)


# my_lib_hybrid.so:
# 	$(CC_HYBRID) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ADDINS)


# app_sandboxed: my_lib.so
# 	$(CXX_PURE) -std=c++17 app_sandboxed.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

# app_sandboxed_better: my_lib.so
# 	$(CXX_PURE) -std=c++17 app_sandboxed_better.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

$(OUTDIR)/app_sandboxed_cheri_pure_noop: app_sandboxed.cpp lib.c
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_NOOP_SANDBOX $^ -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)

# app_sandboxed_better_cheri: 
# 	$(CXX_PURE) -std=c++17 app_sandboxed_better.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)


# app_sandboxed_hybrid: 
# 	$(CXX_HYBRID) -std=c++17 app_sandboxed.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

# app_sandboxed_better_hybrid: 
# 	$(CXX_HYBRID) -std=c++17 app_sandboxed_better.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

all_cheri: $(OUTDIR)/app_sandboxed_cheri_pure_noop

################################# Cheri MSWasm builds ########################################

build/my_lib.mswasm: lib.c lib.h dir
	$(C_TO_MSWASM) $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o mswasm_sandbox_wrapper.o
	$(C_TO_MSWASM) lib.c mswasm_sandbox_wrapper.o -o $@  

build/my_lib.mswasm.c: build/my_lib.mswasm
	$(RWASM) $(RWASM_FLAGS) build/my_lib.mswasm  build/my_lib.mswasm

$(OUTDIR)/my_lib_mswasm_cheri.so: build/my_lib.mswasm.c 
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) --shared -fPIC $< -o $@

# Morello mswasm pure => pure
$(OUTDIR)/app_sandboxed_cheri_mswasm: $(OUTDIR)/my_lib_mswasm_cheri.so app_sandboxed.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -DCHERI_MSWASM_SANDBOX -std=c++17  app_sandboxed.cpp -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)

all_mswasm: $(OUTDIR)/app_sandboxed_cheri_mswasm

clean:
	-rm -rf build 
	-rm -rf $(OUTDIR)  
