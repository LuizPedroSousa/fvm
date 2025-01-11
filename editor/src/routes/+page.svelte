<script lang="ts">
  import Layer from "../components/layers/layer.svelte";
  import SplitLayer from "../components/layers/split-layer.svelte";
  import ViewportLayer from "../views/viewport-layer.svelte";
</script>

<SplitLayer style="height: 400px">
  <Layer minSize={20} maxSize={70}>
    <span>
      1
      <br />
      <em class="specs">I have a min height of 20% &amp; max height of 70%</em>
    </span>
  </Layer>
  <ViewportLayer />
  <Layer maxSize={70}>
    <span>
      3
      <br />
      <em class="specs">I have a max height of 70%</em>
    </span>
  </Layer>
</SplitLayer>

<style>
  * {
    box-sizing: border-box;
    padding: 0;
    margin: 0;
  }

  :root {
    font-family: Inter, Avenir, Helvetica, Arial, sans-serif;
    font-synthesis: none;
    text-rendering: optimizeLegibility;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    -webkit-text-size-adjust: 100%;
  }

  .container canvas {
    width: 800px;
    height: 600px;
  }

  .container strong {
    color: black;
  }
  .container {
    height: 100%;
    width: 100%;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    text-align: center;
  }
</style>
