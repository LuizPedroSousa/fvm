<script lang="ts">
  import Layer, { type LayerProps } from "../components/layers/layer.svelte";

  interface ViewportLayerProps {
    layer: LayerProps | undefined;
  }

  const { layer }: ConsoleLayerProps = $props();

  const parsedLayer: LayerProps = {
    title: "Console",
    ...layer,
  };
</script>

<Layer {...parsedLayer}>test</Layer>
