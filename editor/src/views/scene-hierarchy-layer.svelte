<script lang="ts">
  import * as Collapsible from "$components/ui/collapsible";
  import { Input } from "$components/ui/input";
  import { cn } from "$helpers/styling";
  import Icon from "@iconify/svelte";
  import { getContext } from "svelte";
  import type { LayerProps } from "../components/layers/layer.svelte";
  import Layer from "../components/layers/layer.svelte";
  import * as ContextMenu from "../components/ui/context-menu";
  import { ScrollArea } from "../components/ui/scroll-area";
  import type { EngineContext, Entity } from "../contexts/engine.context";
  import ContextMenuItem from "../templates/context-menu-item.svelte";

  interface SceneHierarchyLayerProps {
    layer: Partial<LayerProps> | undefined;
  }

  const { layer }: SceneHierarchyLayerProps = $props();

  const parsedLayer: LayerProps = {
    panes: [
      {
        id: "scene-hierachy",
        name: "Scene Hierarchy",
        icon: "icon-park-solid:tree-list",
      },
    ],
    ...layer,
  };

  const { scene, set_selected_entity, setActivePaneId } =
    getContext<EngineContext>("engine");

  const context_options = [
    {
      name: "Add Entity",
      shortcut: "ad",
      icon: "mdi:plus-box",
      items: [
        {
          name: "3D Object",
          icon: "mdi:cube-outline",
          items: [
            { name: "Cube", icon: "mdi:cube" },
            { name: "Sphere", icon: "mdi:sphere" },
            { name: "Capsule", icon: "mdi:pill" },
          ],
        },
        {
          name: "Light",
          icon: "mdi:lightbulb-on-outline",
          items: [
            { name: "Point Light", icon: "mdi:lightbulb-outline" },
            { name: "Directional Light", icon: "mdi:weather-sunny" },
            { name: "Spot Light", icon: "mdi:spotlight-beam" },
          ],
        },
        {
          type: "separator",
        },
        { name: "Camera", icon: "mdi:camera-outline" },
        { name: "Empty Object", icon: "mdi:vector-square" },
      ],
    },
    {
      name: "Transform",
      shortcut: "tr",
      icon: "mdi:axis-arrow",
      items: [
        { name: "Reset Position", icon: "mdi:map-marker" },
        { name: "Reset Rotation", icon: "mdi:rotate-3d" },
        { name: "Reset Scale", icon: "mdi:arrow-expand" },
        { name: "Reset All", icon: "mdi:restore" },
      ],
    },
    {
      name: "Hierarchy",
      shortcut: "hr",
      icon: "mdi:family-tree",
      items: [
        { name: "Duplicate", icon: "mdi:content-duplicate" },
        { name: "Delete", icon: "mdi:trash-can-outline" },
        { name: "Rename", icon: "mdi:pencil" },
        { name: "Move to Parent", icon: "mdi:arrow-up-bold-box" },
        { name: "Move to Root", icon: "mdi:arrow-down-bold-box" },
      ],
    },
    {
      name: "Visibility",
      shortcut: "vs",
      icon: "mdi:eye-outline",
      items: [
        { name: "Toggle Visibility", icon: "mdi:eye-off-outline" },
        { name: "Lock/Unlock", icon: "mdi:lock-outline" },
      ],
    },
    {
      name: "Components",
      shortcut: "cm",
      icon: "mdi:layers-outline",
      items: [
        { name: "Add Component", icon: "mdi:plus-box-multiple-outline" },
        { name: "Remove All Components", icon: "mdi:delete-sweep-outline" },
      ],
    },
  ];

  let isContextMenuOpen = $state(false);
  let selectedContextEntity: any = $state(null);

  function openContextMenu(entity: any) {
    setActivePaneId("scene-hierachy");
    selectedContextEntity = entity;
    isContextMenuOpen = true;
  }

  const handleSelectEntity = (entity: Entity) => {
    set_selected_entity(entity.id);
    setActivePaneId("scene-hierachy");
    selectedContextEntity = null;
  };
</script>

<Layer {...parsedLayer}>
  <div class="px-2 mb-1 mt-3">
    <Input placeholder="Search" class="rounded-sm h-8" />
  </div>

  <ContextMenu.Root bind:open={isContextMenuOpen}>
    <ContextMenu.Content class="min-w-80 bg-bunker-950">
      {#if !!selectedContextEntity}
        <h2 class="ml-6 text-sunset-orange-500">
          {selectedContextEntity?.name}
        </h2>
      {/if}
      <ContextMenu.Separator />
      {#each context_options as option}
        <ContextMenuItem item={option} />
      {/each}
    </ContextMenu.Content>

    <ScrollArea class="h-full w-full py-1">
      {#each Object.values($scene.entities) as entity}
        <Collapsible.Root class="px-2 w-full">
          <ContextMenu.Trigger
            on:contextmenu={() => openContextMenu(entity)}
            class="flex"
          >
            <Collapsible.Trigger
              class={cn(
                "px-2",
                selectedContextEntity?.id === entity?.id &&
                  "bg-sunset-orange-500",
              )}
            >
              <Icon icon="weui:arrow-filled" />
            </Collapsible.Trigger>
            <button
              class={cn(
                "flex w-full items-center space-x-1 px-0.5 transition duration-100 focus:bg-sunset-orange-500 rounded-sm py-0.5 hover:opacity-60",

                selectedContextEntity?.id === entity?.id &&
                  "bg-sunset-orange-500",
              )}
              onclick={() => handleSelectEntity(entity)}
            >
              <div class="flex items-center space-x-1">
                <span class="icon">
                  <Icon icon="mdi:cube-outline" width="22" />
                </span>

                <strong class="title">
                  {entity.name}
                </strong>
              </div>
            </button>
          </ContextMenu.Trigger>
          <Collapsible.Content>Test</Collapsible.Content>
        </Collapsible.Root>
      {/each}
    </ScrollArea>
  </ContextMenu.Root>
</Layer>

<style lang="postcss">
  .icon {
    @apply w-6 h-6 flex items-center justify-center;
  }

  .title {
    @apply text-sm;
  }
</style>
