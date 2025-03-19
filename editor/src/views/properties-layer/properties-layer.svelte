<script lang="ts">
  import { getContext } from "svelte";
  import Layer, { type LayerProps } from "../../components/layers/layer.svelte";
  import type { Component, EngineContext } from "../../contexts/engine.context";
  import { Collapsible } from "bits-ui";
  import Icon from "@iconify/svelte";
  import TransformComponent from "./components/transform-component.svelte";
  import RigidBodyComponent from "./components/rigidbody-component.svelte";
  import ResourceComponent from "./components/resource-component.svelte";
  import { Checkbox } from "$components/ui/checkbox";
  import { Input } from "$components/ui/input";
  import { ScrollArea } from "$components/ui/scroll-area";
  import MaterialComponent from "./components/material-component.svelte";

  interface ViewportLayerProps {
    layer: LayerProps | undefined;
  }

  const { layer }: ViewportLayerProps = $props();

  const { scene, selected_entity_id } = getContext<EngineContext>("engine");

  const parsedLayer: LayerProps = {
    panes: [
      {
        id: "properties",
        name: "Properties",
        icon: "material-symbols:info",
      },
    ],
    ...layer,
  };

  let entity = $derived($scene?.entities?.[$selected_entity_id]);

  let components: Record<string, any> = {
    transform: {
      name: "Transform",
      icon: "tabler:gizmo",
      content: TransformComponent,
    },
    material: {
      name: "Material",
      icon: "arcticons:nyon-material-you-icons",
      content: MaterialComponent,
    },
    "rigid-body": {
      name: "RigidBody",
      icon: "game-icons:half-body-crawling",
      content: RigidBodyComponent,
    },
    resource: {
      name: "Resources",
      icon: "grommet-icons:resources",
      content: ResourceComponent,
    },
  };

  let entity_components: Component[] = $state([]);

  $effect(() => {
    entity_components =
      entity?.components
        ?.filter((component) => components?.[component?.name])
        .map((component) => ({
          ...components[component.name],
          data: component,
        })) || [];
  });
</script>

<Layer {...parsedLayer}>
  {#if entity}
    <div class="header space-x-4">
      <span>
        <Icon icon="mdi:cube-outline" width="26" />
      </span>

      <div class="flex items-center space-x-2">
        <div class="flex items-center space-x-2 w-full">
          <Checkbox class="h-4 w-4" />

          <Input
            placeholder="GameObject Name"
            type="text"
            class="w-full max-w-full h-6"
            bind:value={entity.name}
          />
        </div>

        <div class="flex items-center space-x-2">
          <span class="font-bold text-sm">STATIC</span>
          <Checkbox class="h-4 w-4" />
        </div>
      </div>
    </div>
  {/if}

  {#each entity_components as component}
    <Collapsible.Root class="w-full">
      <ScrollArea>
        <div class="component">
          <Collapsible.Trigger
            class="w-full h-full bg-bunker-900 flex items-center space-x-2 py-2 px-2"
          >
            <Icon icon="weui:arrow-filled" />
            <div class="flex items-center space-x-2">
              <Icon icon={component.icon} />
              <Checkbox class="h-4 w-4" bind:checked={component.active} />
              <strong class="title">
                {component?.name}
              </strong>
            </div>
          </Collapsible.Trigger>
        </div>
        <Collapsible.Content>
          <svelte:component this={component?.content} data={component?.data} />
        </Collapsible.Content>
      </ScrollArea>
    </Collapsible.Root>
  {/each}
</Layer>

<style lang="postcss">
  .component {
    @apply w-full flex items-center justify-between rounded-md;
  }

  .component:focus {
    @apply bg-sunset-orange-600 ring-cabaret-400 ring-2;
  }

  .header {
    @apply w-full bg-bunker-800 px-2 flex items-start py-2;
  }

  .header > div {
    @apply flex items-center w-full;
  }

  .title {
    @apply text-sm font-medium;
  }
</style>
