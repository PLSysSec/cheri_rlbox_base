#include "lib.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#define RLBOX_SINGLE_THREADED_INVOCATIONS
#include "rlbox.hpp"

#if defined(WASM2C_SANDBOX)
    #include "rlbox_wasm2c_sandbox.hpp"
    typedef rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox> sbox_t;
    #define CREATE_SANDBOX(sbox, path) sbox.create_sandbox(path);
#elif defined(NOOP_SANDBOX)
    #include "rlbox_noop_sandbox.hpp"
    typedef rlbox::rlbox_sandbox<rlbox::rlbox_noop_sandbox> sbox_t;
    #define CREATE_SANDBOX(sbox, path) sbox.create_sandbox(); // if noop sandbox, throw path away
#elif defined(CHERI_NOOP_SANDBOX)
    #include "rlbox_cheri_noop_sandbox.hpp"
    typedef rlbox::rlbox_sandbox<rlbox::rlbox_cheri_noop_sandbox> sbox_t;
    #define CREATE_SANDBOX(sbox, path) sbox.create_sandbox(); // if noop sandbox, throw path away
#elif defined(CHERI_DYLIB_SANDBOX)
    #include "rlbox_cheri_dylib_sandbox.hpp"
    typedef rlbox::rlbox_sandbox<rlbox::rlbox_cheri_dylib_sandbox> sbox_t;
    #define CREATE_SANDBOX(sbox, path) sbox.create_sandbox(path);
#elif defined(CHERI_MSWASM_SANDBOX)
    #include "mswasm/impl.hpp"
    typedef rlbox::rlbox_sandbox<rlbox::rlbox_mswasm_sandbox> sbox_t;
    #define CREATE_SANDBOX(sbox, path) sbox.create_sandbox(path);
#else 
    static_assert(false, "No sandbox type defined");
#endif

//Callback on completion of library function
// void on_completion(char* result) {
void on_completion(sbox_t& _,
            rlbox::tainted<char*, rlbox::rlbox_mswasm_sandbox> tainted_str){
    char result_str[100];
    auto result = tainted_str.UNSAFE_unverified();
    strcpy(result_str, result);
    std::cout << "Done! Result = " << result_str << "\n";
}

auto copy_str_to_sandbox(sbox_t &sandbox, char* str){
    size_t str_size = strlen(str) + 1;
    auto str_tainted = sandbox.malloc_in_sandbox<char>(str_size);
    // copy to sandbox
    std::strncpy(str_tainted.unverified_safe_pointer_because(str_size, "writing to region"), str, str_size);
    return str_tainted;
}

int main(int argc, char const *argv[])
{
    sbox_t sandbox;
    sandbox.create_sandbox("./my_lib.so");

    //check for input from command line
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " STRING_TO_HASH RESULT_MESSAGE" << std::endl;
        return 1;
    }

    auto on_completion_cb = sandbox.register_callback(on_completion);

    char* copy_str = (char*)argv[1];
    auto tainted_copy_str = copy_str_to_sandbox(sandbox, copy_str);
   
    char* result_str = (char*)argv[2];
    auto tainted_result_str = copy_str_to_sandbox(sandbox, result_str);

    sandbox.invoke_sandbox_function(print_version);
    long long hash = sandbox.invoke_sandbox_function(get_hash, tainted_copy_str,
    on_completion_cb, tainted_result_str).UNSAFE_unverified();
    
    printf("Hash = %llx\n", hash);

    sandbox.free_in_sandbox(tainted_copy_str);
    sandbox.free_in_sandbox(tainted_result_str);

    sandbox.destroy_sandbox();

    return 0;
}
