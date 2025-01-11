import { createRequire } from "node:module";

const require = createRequire(import.meta.url);

const { parentPort } = require("worker_threads");
const sharedMemory = require("./build/Release/share-memory");

// Infinite loop that reads shared memory and sends it to the parent thread
function processFramebuffer() {
  while (true) {
    console.log("test")
    //const framebuffer = sharedMemory.read_shared_memory();
    
    //// Send framebuffer to the parent thread
    //parentPort.postMessage({ type: "framebuffer", data: framebuffer });
    
    // Wait for 100ms (similar to setInterval)
    //const start = Date.now();
    //while (Date.now() - start < 100) {
    //  // Just busy-wait to simulate the delay
    //}
  }
}

// Start processing the framebuffer
processFramebuffer();