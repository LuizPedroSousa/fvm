<script lang="ts">
  import { getContext, onMount } from "svelte";
  import Layer, { type LayerProps } from "../components/layers/layer.svelte";
  import type { EngineContext } from "../contexts/engine.context";

  let canvas: HTMLCanvasElement;
  let fps = $state(0);
  let resize = false;

  const framebuffer = {
    width: 1920,
    height: 1080,
  };

  let frameCount = 0;
  let lastTime = performance.now();

  interface ViewportLayerProps {
    layer: LayerProps | undefined;
  }

  export const layer: ViewportLayerProps["layer"] = {
    panes: [
      {
        id: "viewport",
        name: "Viewport",
        icon: "tabler:video-filled",
      },
    ],
  };

  let engine = getContext<EngineContext>("engine");

  let gl: WebGLRenderingContext;
  let newFrameAvailable = false;
  let updateInterval = 1000;
  let latestFramebufferBinary: Uint8Array | null = null;
  let texture: WebGLTexture;

  function setupWebGL() {
    gl = canvas.getContext("webgl") as WebGLRenderingContext;
    if (!gl) {
      console.error("WebGL not supported");
      return;
    }

    // Vertex shader
    const vsSource = `
    attribute vec2 position;
    varying vec2 texture_coordinates;
    void main() {
      gl_Position = vec4(position, 0.0, 1.0);
      texture_coordinates = (position + 1.0) / 2.0; // 0 to 1 range
    }
  `;

    // Fragment shader
    const fsSource = `
    precision mediump float;
    varying vec2 texture_coordinates;
    uniform sampler2D u_texture;
    void main() {
      gl_FragColor = texture2D(u_texture, texture_coordinates);
    }
  `;

    const vertexShader = gl.createShader(gl.VERTEX_SHADER);

    if (!vertexShader) {
      return;
    }

    const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);

    if (!fragmentShader) {
      return;
    }

    gl.shaderSource(vertexShader, vsSource);
    gl.compileShader(vertexShader);

    gl.shaderSource(fragmentShader, fsSource);
    gl.compileShader(fragmentShader);

    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    gl.useProgram(program);

    // Quad vertices (-1 to 1)
    const vertices = new Float32Array([-1, -1, 1, -1, -1, 1, 1, 1]);

    const buffer = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

    const positionLoc = gl.getAttribLocation(program, "position");
    gl.enableVertexAttribArray(positionLoc);
    gl.vertexAttribPointer(positionLoc, 2, gl.FLOAT, false, 0, 0);

    // Texture setup
    const texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

    return { program, texture };
  }

  function resizeCanvas() {
    const container = canvas.parentElement;
    if (!container) return;

    const containerWidth = container.clientWidth;
    const containerHeight = container.clientHeight;
    const framebufferRatio = framebuffer.width / framebuffer.height;
    const containerRatio = containerWidth / containerHeight;

    let canvasWidth, canvasHeight;

    if (containerRatio > framebufferRatio) {
      canvasHeight = containerHeight;
      canvasWidth = canvasHeight * framebufferRatio;
    } else {
      canvasWidth = containerWidth;
      canvasHeight = canvasWidth / framebufferRatio;
    }

    canvas.style.width = `${canvasWidth}px`;
    canvas.style.height = `${canvasHeight}px`;

    gl.canvas.width = framebuffer.width;
    gl.canvas.height = framebuffer.height;
  }

  function renderFrame() {
    if (!latestFramebufferBinary) return;

    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(
      gl.TEXTURE_2D,
      0,
      gl.RGBA,
      framebuffer.width,
      framebuffer.height,
      0,
      gl.RGBA,
      gl.UNSIGNED_BYTE,
      latestFramebufferBinary,
    );

    gl.viewport(0, 0, framebuffer.width, framebuffer.height);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

    // FPS calculation
    frameCount++;
    const currentTime = performance.now();
    const elapsedTime = currentTime - lastTime;
    if (elapsedTime >= updateInterval) {
      fps = Math.round((frameCount * 1000) / elapsedTime);
      frameCount = 0;
      lastTime = currentTime;
    }

    latestFramebufferBinary = null;
    newFrameAvailable = false;
  }

  onMount(() => {
    const webgl = setupWebGL();
    if (!webgl) return;
    texture = webgl.texture;

    resizeCanvas();

    window.addEventListener("resize", () => {
      resize = false;
    });

    return () => {
      window.removeEventListener("resize", resizeCanvas);
    };
  });

  engine.socket_event.subscribe((event) => {
    if (event && event.topic === "framebuffer") {
      latestFramebufferBinary = event.content;
      if (!newFrameAvailable) {
        newFrameAvailable = true;

        resizeCanvas();

        if (!resize) {
          resizeCanvas();
          resize = true;
        }
        renderFrame();
      }
    }
  });

  let key_events: any = $state({});
  let isCanvasActive = $state(false);

  const handleKeydown = (event: KeyboardEvent) => {
    if (!isCanvasActive) return;

    const key = key_events?.[event.keyCode];

    if (!key || key?.state !== "press") {
      key_events[event.keyCode] = {
        ...key_events[event.keyCode],
        state: "press",
      };

      engine?.send_event({
        keyCode: event.keyCode,
        type: "keyboard",
        action: "press",
      });
    }
  };

  const handleKeyup = (event: KeyboardEvent) => {
    if (event.key === "Escape" && isCanvasActive) {
      isCanvasActive = false;
    }
    if (!isCanvasActive) return;

    const key = key_events?.[event.keyCode];

    if (!key || key !== "release") {
      key_events[event.keyCode] = {
        ...key_events[event.keyCode],
        state: "release",
      };

      engine?.send_event({
        keyCode: event.keyCode,
        type: "keyboard",
        action: "release",
      });
    }
  };

  document.addEventListener("mousemove", (event) => {
    if (!isCanvasActive || document.pointerLockElement !== canvas) return;

    engine.send_event({
      type: "mouse",
      action: "move",
      x: event.movementX,
      y: event.movementY,
    });
  });

  window.addEventListener("keydown", handleKeydown);
  window.addEventListener("keyup", handleKeyup);

  $effect(() => {
    if (isCanvasActive) {
      engine.setActivePaneId("viewport");
    }
  });
  const handleActivateCanvas = async () => {
    isCanvasActive = true;

    canvas.requestPointerLock();
  };
</script>

<Layer {...layer}>
  <div class="viewport-container">
    <canvas onclick={handleActivateCanvas} bind:this={canvas}></canvas>
    <div class="fps-counter">FPS: {fps}</div>
  </div>
</Layer>

<style>
  .viewport-container {
    position: relative;
    width: 100%;
    height: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  canvas {
    width: 100%;
    height: 100%;
    margin: auto;
  }

  .fps-counter {
    position: absolute;
    top: 10px;
    right: 10px;
    color: white;
    background: rgba(0, 0, 0, 0.7);
    padding: 5px 10px;
    border-radius: 3px;
    font-family: monospace;
  }
</style>
