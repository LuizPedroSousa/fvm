import { useEffect, useRef, useState } from "react";
import reactLogo from "./assets/react.svg";
import viteLogo from "/electron-vite.animate.svg";
import "./App.css";

const Canvas = () => {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  const [src, setSrC] = useState();

  useEffect(() => {
    const canvas = canvasRef.current;
    const ctx = canvas.getContext("2d");

    if (!ctx) {
      console.error("Canvas 2D context not supported");
      return;
    }

    // Create an ImageData object to hold the pixel data
    const imageData = ctx.createImageData(800, 600);

    // Function to update the framebuffer
    function updateFramebuffer(framebufferBinary: any) {
      // Parse the binary data into a Uint8Array (if it's not already)
      const binaryString = atob(framebufferBinary);

      const uint8Array = new Uint8Array(binaryString.length);
      for (let i = 0; i < binaryString.length; i++) {
        uint8Array[i] = binaryString.charCodeAt(i);
      }

      if (uint8Array.length !== 800 * 600 * 4) {
        console.error("Invalid framebuffer data length:", uint8Array.length);
        return;
      }

      // Fill the imageData's data array with the framebuffer data
      for (let i = 0; i < uint8Array.length; i++) {
        imageData.data[i] = uint8Array[i];
      }

      // Put the ImageData on the canvas (at position 0,0)
      ctx.putImageData(imageData, 0, 0);

      // Redraw the canvas
      requestAnimationFrame(() => updateFramebuffer(framebufferBinary));
    }

    // Start the continuous update of the framebuffer
    updateFramebuffer(null); // Start with no data, waiting for the first update

    // Listen for the framebuffer event and update the canvas
    window.ipcRenderer.on("framebuffer", (event, data) => {
      setSrC(data);
      updateFramebuffer(data);
    });

    // Cleanup listener on component unmount
    return () => {
      window.ipcRenderer.off("framebuffer", () => {});
    };
  }, []);

  return (
    <>
      <canvas ref={canvasRef} width={800} height={600}></canvas>
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
