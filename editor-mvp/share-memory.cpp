#include <node_api.h>
#include <v8.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "iostream"
#include "chrono"
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#define SHM_NAME "/framebuffer_shm"
#define WIDTH 800
#define HEIGHT 600
#define PIXEL_SIZE 4  // RGBA

static int fd = -1;
static void* ptr = nullptr;

void FinalizeSharedMemory(napi_env env, void* finalize_data, void* finalize_hint) {
  munmap(finalize_data, WIDTH * HEIGHT * PIXEL_SIZE);
  close(fd);
}

napi_value ReadSharedMemory(napi_env env, napi_callback_info info) {
  auto start_time = std::chrono::high_resolution_clock::now();


  if (fd == -1) {
    std::cout << "Opening fd" << std::endl;
    fd = shm_open(SHM_NAME, O_RDWR, 0666);

    if (fd == -1) {
      napi_throw_error(env, nullptr, "Failed to open shared memory");
      std::cout << "Failed to open shared memory" << std::endl;
      return nullptr;
    }

    if (ftruncate(fd, WIDTH * HEIGHT * PIXEL_SIZE) == -1) {

      napi_throw_error(env, nullptr, "Error setting size of shared memory");
      std::cerr << "Error setting size of shared memory" << std::endl;
      return nullptr;
    }

    ptr = mmap(nullptr, WIDTH * HEIGHT * PIXEL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
      std::cout << "Failed to map shared memory" << std::endl;
      napi_throw_error(env, nullptr, "Failed to open shared memory");
      return nullptr;
    }
  }

  if (ptr == nullptr || ptr == MAP_FAILED) {
    napi_throw_error(env, nullptr, "Invalid shared memory pointer");
    return nullptr;
  }


  napi_value arrayBuffer;

  // napi_status status = napi_create_external_arraybuffer(env,
  //   ptr,
  //   WIDTH * HEIGHT * PIXEL_SIZE,
  //   FinalizeSharedMemory,
  //   nullptr,
  //   &arrayBuffer);

  // if (status != napi_ok) {
  //   const napi_extended_error_info* error_message;
  //   napi_get_last_error_info(env, &error_message);
  //   std::cerr << "napi_create_external_arraybuffer failed: " << error_message->error_message << std::endl;

  //   napi_throw_error(env, nullptr, "Failed to create ArrayBuffer");
  //   return nullptr;
  // }




  // Instead of creating an external ArrayBuffer, pass the raw pointer and size
  napi_status status = napi_create_arraybuffer(env,
    WIDTH * HEIGHT * PIXEL_SIZE, // Size of the shared memory
    &ptr,
    &arrayBuffer);

  if (status != napi_ok) {
    napi_throw_error(env, nullptr, "Failed to create ArrayBuffer");
    return nullptr;
  }

  auto end_time = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << "Time taken: " << duration.count() << " microseconds." << std::endl;

  return arrayBuffer;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor desc = { "read_shared_memory", 0, ReadSharedMemory, 0, 0, 0, napi_default, 0 };
  napi_define_properties(env, exports, 1, &desc);
  return exports;
}


NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
