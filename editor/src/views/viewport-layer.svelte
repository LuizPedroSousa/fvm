<script lang="ts">
  import { onMount } from "svelte";
  import Layer from "../components/layers/layer.svelte";

  let canvas: any;
  let ws: any;

  onMount(() => {
    const startTime = performance.now();

    const ctx = canvas.getContext("2d");

    if (!ctx) {
      console.error("Canvas 2D context not supported");
      return;
    }

    // Create an ImageData object to hold the pixel data
    const imageData = ctx.createImageData(1920, 1080);

    // Function to update the framebuffer
    function updateFramebuffer(framebufferBinary: any) {
      const uint8Array = new Uint8Array(framebufferBinary);

      if (uint8Array.length !== 1920 * 1080 * 4) {
        console.error("Invalid framebuffer data length:", uint8Array.length);
        return;
      }

      for (let i = 0; i < uint8Array.length; i++) {
        imageData.data[i] = uint8Array[i];
      }

      ctx.putImageData(imageData, 0, 0);

      const endTime = performance.now();
      //     console.log(`Frame rendered in ${(endTime - startTime).toFixed(2)} ms`);
    }

    // Set up WebSocket connection
    ws = new WebSocket("ws://localhost:9001");
    ws.binaryType = "arraybuffer";

    ws.onopen = () => {
      console.log("WebSocket connection established");
    };

    ws.onmessage = (event: any) => {
      if (event.data instanceof ArrayBuffer) {
        updateFramebuffer(event.data);
      } else {
        console.warn("Received non-binary data");
      }
    };

    ws.onerror = (error: any) => {
      console.error("WebSocket error:", error);
    };

    ws.onclose = () => {
      console.log("WebSocket connection closed");
    };

    return () => {
      ws.close();
    };
  });
</script>

<Layer>
  <canvas bind:this={canvas} width="800" height="600"></canvas>
</Layer>
