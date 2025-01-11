import { useEffect, useRef, useState } from "react";
import reactLogo from "./assets/react.svg";
import viteLogo from "/electron-vite.animate.svg";
import "./App.css";

const Canvas = () => {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const startTime = performance.now();

    const canvas = canvasRef.current;
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
      console.log(`Frame rendered in ${(endTime - startTime).toFixed(2)} ms`);
    }

    const ws = new WebSocket("ws://localhost:9001");

    ws.binaryType = "arraybuffer";

    ws.onopen = () => {
      console.log("WebSocket connection established");
    };

    ws.onmessage = (event) => {
      if (event.data instanceof ArrayBuffer) {
        updateFramebuffer(event.data);
      } else {
        console.warn("Received non-binary data");
      }
    };

    ws.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

    ws.onclose = () => {
      console.log("WebSocket connection closed");
    };

    return () => {
      ws.close();
    };
  }, []);

  return (
    <>
      <canvas ref={canvasRef} width={1920} height={1080}></canvas>
    </>
  );
};

function App() {
  const [count, setCount] = useState(0);

  return (
    <>
      <div>
        <a href="https://electron-vite.github.io" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <Canvas />
      <h1>Vite + React</h1>
      <div className="card">
        <button onClick={() => setCount((count) => count + 1)}>
          count is {count}
        </button>
        <p>
          Edit <code>src/App.tsx</code> and save to test HMR
        </p>
      </div>
      <p className="read-the-docs">
        Click on the Vite and React logos to learn more
      </p>
    </>
  );
}

export default App;
