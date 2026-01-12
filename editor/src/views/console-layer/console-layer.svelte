<script lang="ts">
  import * as Tabs from "$components/ui/tabs";
  import Layer, { type LayerProps } from "../../components/layers/layer.svelte";
  import DataTable from "./tables/data-table.svelte";

  interface ConsoleLayerProps {
    layer: LayerProps | undefined;
  }

  const { layer }: ConsoleLayerProps = $props();

  const parsedLayer: LayerProps = {
    title: "Console",
    id: "console",
    ...layer,
  };
</script>

<Layer {...parsedLayer}>
  <Tabs.Root value="table">
    <Tabs.List class="grid w-[200px] grid-cols-2 ml-4 mt-2">
      <Tabs.Trigger value="list">List View</Tabs.Trigger>
      <Tabs.Trigger value="table">Table View</Tabs.Trigger>
    </Tabs.List>

    <Tabs.Content value="list"></Tabs.Content>
    <Tabs.Content value="table" class="w-full px-4">
      <DataTable />
    </Tabs.Content>
  </Tabs.Root>
</Layer>
