<script lang="ts">
  import LayerHandle from "$components/layers/layer-handle.svelte";
  import Layer from "$components/layers/layer.svelte";
  import SplitLayer from "../components/layers/split-layer.svelte";
  import ConsoleLayer from "../views/console-layer.svelte";
  import PropertiesLayer from "../views/properties-layer.svelte";
  import SceneHierarchyLayer from "../views/scene-hierarchy-layer.svelte";
  import ViewportLayer from "../views/viewport-layer.svelte";
</script>

<main class="wrapper">
  <SplitLayer direction="vertical">
    <Layer>
      <SplitLayer>
        <SceneHierarchyLayer layer={{ minSize: 20, defaultSize: 20 }} />
        <LayerHandle />
        <ViewportLayer />
        <LayerHandle />
        <PropertiesLayer layer={{ minSize: 20, defaultSize: 20 }} />
      </SplitLayer>
    </Layer>
    <LayerHandle />
    <ConsoleLayer layer={{ minSize: 20, defaultSize: 20 }} />
  </SplitLayer>
</main>

<style>
  .wrapper {
    height: 100vh;
    display: flex;
    flex-direction: column;
    width: 100vw;
  }
</style>
