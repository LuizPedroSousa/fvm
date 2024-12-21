#include <node.h>
#include <v8.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "iostream"

#define SHM_NAME "/framebuffer_shm"
#define WIDTH 800
#define HEIGHT 600
#define PIXEL_SIZE 4  // RGBA


std::string base64_encode(const uint8_t* data, size_t length) {
  static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string result;
  int val = 0;
  int valb = -6;

  for (size_t i = 0; i < length; i++) {
    val = (val << 8) + data[i];
    valb += 8;
    while (valb >= 0) {
      result.push_back(base64_table[(val >> valb) & 0x3F]);
      valb -= 6;
    }
  }

  if (valb > -6) {
    result.push_back(base64_table[((val << 8) >> (valb + 8)) & 0x3F]);
  }

  while (result.size() % 4) {
    result.push_back('=');
  }

  return result;
}


void ReadSharedMemory(const v8::FunctionCallbackInfo<v8::Value>& args) {

  v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[0]);

  int fd = shm_open(SHM_NAME, O_RDWR, 0666);

  if (fd == -1) {
    // Set the error argument as the first callback argument
    v8::Isolate* isolate = args.GetIsolate();
    v8::Local<v8::String> error_msg = v8::String::NewFromUtf8(isolate, "Failed to open shared memory").ToLocalChecked();
    args.GetReturnValue().Set(v8::Null(isolate));
    args.GetReturnValue().Set(error_msg);
    std::cout << "Failed to open shared memory" << std::endl;
    return;
  }

  // Set the size of the shared memory segment
  if (ftruncate(fd, WIDTH * HEIGHT * PIXEL_SIZE) == -1) {
    std::cerr << "Error setting size of shared memory" << std::endl;
    return;
  }

  // Map the shared memory to the process's address space
  void* ptr = mmap(NULL, WIDTH * HEIGHT * PIXEL_SIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED) {
    std::cout << "Failed to map shared memory" << std::endl;
    args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), "Failed to map shared memory").ToLocalChecked());
    return;
  }

  // Read the data from shared memory
  uint8_t* shared_data = (uint8_t*)ptr;

  auto encoded = base64_encode(shared_data, WIDTH * HEIGHT * PIXEL_SIZE);

  args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), encoded.c_str()).ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "read_shared_memory", ReadSharedMemory);
}

NODE_MODULE(shared_memory, Init)