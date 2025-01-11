cmd_Release/obj.target/share-memory/share-memory.o := g++ -o Release/obj.target/share-memory/share-memory.o ../share-memory.cpp '-DNODE_GYP_MODULE_NAME=share-memory' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-D_GLIBCXX_USE_CXX11_ABI=1' '-DELECTRON_ENSURE_CONFIG_GYPI' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-DUSING_ELECTRON_CONFIG_GYPI' '-DV8_COMPRESS_POINTERS' '-DV8_COMPRESS_POINTERS_IN_ISOLATE_CAGE' '-DV8_31BIT_SMIS_ON_64BIT_ARCH' '-DV8_ENABLE_SANDBOX' '-D__STDC_FORMAT_MACROS' '-DOPENSSL_NO_PINSHARED' '-DOPENSSL_THREADS' '-DOPENSSL_NO_ASM' '-DBUILDING_NODE_EXTENSION' -I/home/bluesy/.electron-gyp/30.5.1/include/node -I/home/bluesy/.electron-gyp/30.5.1/src -I/home/bluesy/.electron-gyp/30.5.1/deps/openssl/config -I/home/bluesy/.electron-gyp/30.5.1/deps/openssl/openssl/include -I/home/bluesy/.electron-gyp/30.5.1/deps/uv/include -I/home/bluesy/.electron-gyp/30.5.1/deps/zlib -I/home/bluesy/.electron-gyp/30.5.1/deps/v8/include  -fPIC -pthread -Wall -Wextra -Wno-unused-parameter -m64 -O3 -fno-omit-frame-pointer -fno-rtti -fno-exceptions -std=gnu++17 -MMD -MF ./Release/.deps/Release/obj.target/share-memory/share-memory.o.d.raw   -c
Release/obj.target/share-memory/share-memory.o: ../share-memory.cpp \
 /home/bluesy/.electron-gyp/30.5.1/include/node/node_api.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/js_native_api.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/js_native_api_types.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/node_api_types.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/cppgc/common.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8config.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-array-buffer.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-local-handle.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-handle-base.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-internal.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8config.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-object.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-maybe.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-persistent-handle.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-weak-callback-info.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-primitive.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-data.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-value.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-traced-handle.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-container.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-context.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-snapshot.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-isolate.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-callbacks.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-promise.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-debug.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-script.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-memory-span.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-message.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-embedder-heap.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-function-callback.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-microtask.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-statistics.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-unwinder.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-embedder-state-scope.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-date.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-exception.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-extension.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-external.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-function.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-template.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-initialization.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-platform.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-source-location.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-json.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-locker.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-microtask-queue.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-primitive-object.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-proxy.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-regexp.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-typed-array.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-value-serializer.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-version.h \
 /home/bluesy/.electron-gyp/30.5.1/include/node/v8-wasm.h
../share-memory.cpp:
/home/bluesy/.electron-gyp/30.5.1/include/node/node_api.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/js_native_api.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/js_native_api_types.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/node_api_types.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/cppgc/common.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8config.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-array-buffer.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-local-handle.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-handle-base.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-internal.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8config.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-object.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-maybe.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-persistent-handle.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-weak-callback-info.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-primitive.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-data.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-value.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-traced-handle.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-container.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-context.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-snapshot.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-isolate.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-callbacks.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-promise.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-debug.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-script.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-memory-span.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-message.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-embedder-heap.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-function-callback.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-microtask.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-statistics.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-unwinder.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-embedder-state-scope.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-date.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-exception.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-extension.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-external.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-function.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-template.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-initialization.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-platform.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-source-location.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-json.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-locker.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-microtask-queue.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-primitive-object.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-proxy.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-regexp.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-typed-array.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-value-serializer.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-version.h:
/home/bluesy/.electron-gyp/30.5.1/include/node/v8-wasm.h:
