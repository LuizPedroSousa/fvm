<script lang="ts">
  import { Input } from "$components/ui/input";
  import type { Vector3 } from "../models/math";

  export let vector: Vector3;
  export let name: string;

  let isDragging = false;
  let startX: number;
  let startValue: number;
  let draggingAxis: "x" | "y" | "z" | null = null;

  function startDrag(e: MouseEvent, axis: "x" | "y" | "z") {
    isDragging = true;
    startX = e.clientX;
    startValue = vector[axis];
    draggingAxis = axis;
    document.addEventListener("mousemove", drag);
    document.addEventListener("mouseup", stopDrag);
  }

  function drag(e: MouseEvent) {
    if (!isDragging || !draggingAxis) return;
    const delta = e.clientX - startX;
    const sensitivity = 0.01; // Adjust this value to control drag speed
    vector[draggingAxis] = startValue + delta * sensitivity;
  }

  function stopDrag() {
    isDragging = false;
    draggingAxis = null;
    document.removeEventListener("mousemove", drag);
    document.removeEventListener("mouseup", stopDrag);
  }
</script>

<div class="container">
  <strong>{name}</strong>
  <div class="space-x-2">
    <div class="axis">
      <span class="bg-red-500" on:mousedown={(e) => startDrag(e, "x")}>X</span>
      <Input
        class="rounded-l-none h-full"
        placeholder="X"
        type="number"
        bind:value={vector.x}
      />
    </div>

    <div class="axis">
      <span class="bg-green-500" on:mousedown={(e) => startDrag(e, "y")}>Y</span
      >
      <Input
        class="rounded-l-none w-full h-full"
        placeholder="Y"
        type="number"
        bind:value={vector.y}
      />
    </div>

    <div class="axis">
      <span class="bg-blue-500" on:mousedown={(e) => startDrag(e, "z")}>Z</span>
      <Input
        class="rounded-l-none w-full h-full"
        placeholder="Z"
        type="number"
        bind:value={vector.z}
      />
    </div>
  </div>
</div>

<style lang="postcss">
  .container {
    @apply w-full p-0 grid grid-cols-[5rem_1fr] items-center space-x-2;
  }

  .container strong {
    @apply block text-xs;
  }

  .container > div:first-of-type {
    @apply mt-1;
  }
  .container > div {
    @apply flex items-center;
  }

  .axis {
    @apply flex items-center w-full h-7;
  }

  .axis span {
    @apply w-8 h-full rounded-l flex items-center justify-center text-white font-bold text-sm cursor-ew-resize select-none;
  }
</style>
