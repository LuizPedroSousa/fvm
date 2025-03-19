<script module lang="ts">
  interface FileTreeProps {
    files: File[];
    handleSelectFile: (File: File) => void;
    selected_file?: File;
  }
</script>

<script lang="ts">
  import Layer, { type LayerProps } from "$components/layers/layer.svelte";
  import Input from "$components/ui/input/input.svelte";
  import * as ContextMenu from "$components/ui/context-menu";

  import { ScrollArea } from "$components/ui/scroll-area";
  import Icon from "@iconify/svelte";
  import * as Collapsible from "$components/ui/collapsible";

  import type { File } from "./content-browser.svelte";
  import { cn } from "$helpers/styling";

  import ContextMenuItem from "$templates/context-menu-item.svelte";

  const { files, handleSelectFile, selected_file }: FileTreeProps = $props();

  const layer: LayerProps = {
    id: "file-tree",
    defaultSize: 10,
    minSize: 10,
  };

  let isContextMenuOpen = $state(false);

  function openContextMenu(file: File) {
    isContextMenuOpen = true;
  }

  const context_options = [
    {
      name: "Novo",
      shortcut: "nw",
      icon: "mdi:plus-box",
      items: [
        { name: "Arquivo", icon: "mdi:file-plus" },
        { name: "Pasta", icon: "mdi:folder-plus" },
      ],
    },
    {
      name: "Gerenciar",
      shortcut: "mg",
      icon: "mdi:folder-cog-outline",
      items: [
        { name: "Renomear", icon: "mdi:pencil" },
        { name: "Duplicar", icon: "mdi:content-duplicate" },
        { name: "Mover", icon: "mdi:folder-move" },
        { name: "Excluir", icon: "mdi:trash-can-outline" },
      ],
    },
    {
      name: "Organizar",
      shortcut: "or",
      icon: "mdi:sort",
      items: [
        { name: "Ordenar por Nome", icon: "mdi:sort-alphabetical-ascending" },
        { name: "Ordenar por Data", icon: "mdi:calendar-clock" },
        { name: "Ordenar por Tipo", icon: "mdi:shape-outline" },
      ],
    },
    {
      name: "Abrir com",
      shortcut: "op",
      icon: "mdi:open-in-app",
      items: [
        { name: "Editor de Texto", icon: "mdi:file-document-edit-outline" },
        { name: "Visualizador de Imagem", icon: "mdi:image" },
        { name: "Executar Script", icon: "mdi:console" },
      ],
    },
    {
      name: "Propriedades",
      shortcut: "pr",
      icon: "mdi:information-outline",
      items: [
        { name: "Detalhes do Arquivo", icon: "mdi:file-find-outline" },
        { name: "Copiar Caminho", icon: "mdi:content-copy" },
        { name: "Abrir no Explorador", icon: "mdi:folder-open-outline" },
      ],
    },
  ];
</script>

<Layer {...layer}>
  <ContextMenu.Root bind:open={isContextMenuOpen}>
    <ContextMenu.Content class="min-w-80 bg-bunker-950">
      {#if !!selected_file}
        <h2 class="ml-6 text-sunset-orange-500">
          {selected_file?.name}
        </h2>
      {/if}
      <ContextMenu.Separator />
      {#each context_options as option}
        <ContextMenuItem item={option} />
      {/each}
    </ContextMenu.Content>

    <ScrollArea class="h-full w-full py-1">
      {#each files as file}
        <Collapsible.Root class="px-2 w-full">
          <ContextMenu.Trigger
            on:contextmenu={() => openContextMenu(file)}
            class="flex"
          >
            {#if file.is_dir}
              <Collapsible.Trigger class={cn("px-2 relative pl-4")}>
                <span class="tree-node"> </span>

                <Icon icon="weui:arrow-filled" />
              </Collapsible.Trigger>
            {/if}
            <button
              class={cn(
                "flex w-full items-center space-x-1 px-0.5 transition focus:bg-sunset-orange-500 rounded-sm py-0.5 hover:opacity-60",

                !file.is_dir && "pl-4",
                selected_file?.name === file?.name && "bg-sunset-orange-500",
              )}
              onclick={() => handleSelectFile(file)}
            >
              <div class="flex items-center space-x-1 relative">
                <span class="icon">
                  <Icon
                    icon={file.is_dir ? "material-symbols:folder" : "mdi:file"}
                    width="22"
                  />
                </span>

                <strong class="title">
                  {file.name}
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
  .tree-node {
    content: " ";
    position: absolute;
    background-color: #313e41;
    left: -0px;
    top: 12px;
    width: 12px;
    height: 2px;
  }
</style>
