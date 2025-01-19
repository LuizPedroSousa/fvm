<script lang="ts">
  import * as Collapsible from "$components/ui/collapsible";
  import type { LayerProps } from "../components/layers/layer.svelte";
  import Layer from "../components/layers/layer.svelte";

  interface SceneHierarchyLayerProps {
    layer: Partial<LayerProps> | undefined;
  }

  const { layer }: SceneHierarchyLayerProps = $props();

  const parsedLayer: LayerProps = {
    title: "Scene Hierarchy",
    ...layer,
  };
</script>

<Layer {...parsedLayer}>
  <Collapsible.Root>
    <Collapsible.Trigger class="Trigger">asd</Collapsible.Trigger>
    <Collapsible.Content>Test</Collapsible.Content>
  </Collapsible.Root>
</Layer>
