<script lang="ts">
  import Layer, { type LayerProps } from "../components/layers/layer.svelte";

  interface ViewportLayerProps {
    layer: LayerProps | undefined;
  }

  const { layer }: ViewportLayerProps = $props();

  const parsedLayer: LayerProps = {
    title: "Properties",
    ...layer,
  };
</script>

<Layer {...parsedLayer}>test</Layer>
