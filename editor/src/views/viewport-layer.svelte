<script lang="ts">
  import { onMount } from "svelte";
  import Layer, { type LayerProps } from "../components/layers/layer.svelte";
  import * as api from "@tauri-apps/api";

  let canvas: any;
  let ws: WebSocket;

  interface ViewportLayerProps {
    layer: LayerProps | undefined;
  }

  export const layer: ViewportLayerProps["layer"] = { title: "Viewport" };

  onMount(() => {
    const startTime = performance.now();

    const framebuffer = {
      width: 1920,
      height: 1080,
    };

    const aspectRatio = framebuffer.width / framebuffer.height;

    const ctx = canvas.getContext("2d");

    if (!ctx) {
      console.error("Canvas 2D context not supported");
      return;
    }

    function resizeCanvas() {
      const container = canvas.parentElement;
      if (!container) return;

      const containerWidth = container.clientWidth;
      const containerHeight = container.clientWidth;

      //console.log(container.width, container.height);
      //console.log(`Container: ${containerWidth}x${containerHeight}`);
      //console.log(`Canvas: ${canvas.style.width}x${canvas.style.height}`);

      const containerAspectRatio = containerWidth / containerHeight;

      ctx.canvas.width = framebuffer.width;
      ctx.canvas.height = framebuffer.height;

      // if (containerAspectRatio > aspectRatio) {
      //   // Container é mais largo, limitar pela altura
      //   canvas.style.width = `${containerHeight * aspectRatio}px`;
      //   canvas.style.height = `${containerHeight}px`;
      // } else {
      //   // Container é mais alto, limitar pela largura
      //   canvas.style.width = `${containerWidth}px`;
      //   canvas.style.height = `${containerWidth / aspectRatio}px`;
      // }
    }

    // Create an ImageData object to hold the pixel data
    const imageData = ctx.createImageData(
      framebuffer.width,
      framebuffer.height,
    );

    // Function to update the framebuffer
    function updateFramebuffer(framebufferBinary: any) {
      resizeCanvas();
      const uint8Array = new Uint8Array(framebufferBinary);

      if (uint8Array.length !== framebuffer.width * framebuffer.height * 4) {
        console.error("Invalid framebuffer data length:", uint8Array.length);
        return;
      }

      const invertedData = new Uint8Array(uint8Array.length);
      const rowSize = framebuffer.width * 4;

      for (let y = 0; y < framebuffer.height; y++) {
        const srcIndex = y * rowSize;
        const destIndex = (framebuffer.height - 1 - y) * rowSize;
        invertedData.set(
          uint8Array.subarray(srcIndex, srcIndex + rowSize),
          destIndex,
        );
      }

      // Atualizar o ImageData com os dados invertidos
      for (let i = 0; i < invertedData.length; i++) {
        imageData.data[i] = invertedData[i];
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
      console.log(event);
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

  let key_events: any = $state({});
  let isCanvasActive = $state(false);
  let isMouseVisible = true;

  let isCursorVisible = true;

  const toggleMouseVisibility = () => {};

  const handleKeydown = (event: KeyboardEvent) => {
    if (!isCanvasActive) return;

    const key = key_events?.[event.keyCode];
    if ((ws.readyState === WebSocket.OPEN && !key) || key.state !== "press") {
      key_events[event.keyCode] = {
        ...key_events[event.keyCode],
        state: "press",
      };

      ws.send(
        JSON.stringify({
          keyCode: event.keyCode,
          type: "keyboard",
          action: "press",
        }),
      );
    }
  };

  const handleKeyup = (event: KeyboardEvent) => {
    if (!isCanvasActive) return;

    const key = key_events?.[event.keyCode];

    if (ws.readyState === WebSocket.OPEN && (!key || key !== "release")) {
      key_events[event.keyCode] = {
        ...key_events[event.keyCode],
        state: "release",
      };

      ws.send(
        JSON.stringify({
          keyCode: event.keyCode,
          type: "keyboard",
          action: "release",
        }),
      );
    }
  };

  document.addEventListener("mousemove", (event) => {
    //   if (!isCanvasActive) return;
    if (ws.readyState === WebSocket.OPEN) {
      ws.send(
        JSON.stringify({
          type: "mouse",
          action: "move",
          x: event.offsetX,
          y: event.offsetY,
        }),
      );
    }
  });

  window.addEventListener("keydown", handleKeydown);

  window.addEventListener("keyup", handleKeyup);

  const handleActivateCanvas = async () => {
    isCanvasActive = true;

    console.log("test");

    const current = api.window.getCurrentWindow();

    await current.setCursorVisible(false);
  };
</script>

<Layer {...layer}>
  <canvas onclick={handleActivateCanvas} bind:this={canvas}></canvas>
</Layer>

<style>
  canvas {
    width: 100%;
    margin: auto;
    height: 100%;
    object-fit: stretch;
  }
</style>
