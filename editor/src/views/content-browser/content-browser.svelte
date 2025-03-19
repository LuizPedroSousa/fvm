<script module lang="ts">
  export interface File {
    name: string;
    is_dir: boolean;
  }
</script>

<script lang="ts">
  import { onMount } from "svelte";
  import { invoke } from "@tauri-apps/api/core";
  import Layer, { type LayerProps } from "../../components/layers/layer.svelte";
  import { writable } from "svelte/store";
  import SplitLayer from "$components/layers/split-layer.svelte";
  import LayerHandle from "$components/layers/layer-handle.svelte";
  import FileTree from "./file-tree.svelte";
  import FolderContent from "./folder-content.svelte";
  import Input from "$components/ui/input/input.svelte";
  import LayerContent from "$components/layers/layer-content.svelte";
  import * as Select from "$components/ui/select";
  import * as Breadcrumb from "$components/ui/breadcrumb";
  import Icon from "@iconify/svelte";
  import IconButton from "./icon-button.svelte";

  // Store for directory contents
  const files = writable<File[]>([]);
  const current_path = writable<string>("/home/bluesy/Desktop/personal/fvm");

  async function loadFolder(path: string = "") {
    try {
      const result = await invoke<File[]>("get_directory_contents", { path });
      console.log(result);
      files.set(result);
      current_path.set(path);
    } catch (error) {
      console.error("Failed to load folder:", error);
    }
  }

  function openFolder(name: string, isDir: boolean) {
    if (isDir) {
      loadFolder(`${$current_path}/${name}`);
    }
  }

  let current_view_id = $state("table");

  let selected_file = $state<File>();

  const handleSelectFile = (file: File) => {
    selected_file = file;
  };

  onMount(() => {
    loadFolder("/home/bluesy/Desktop/personal/fvm");
  });

  const buttons = [
    {
      id: "grid:large",
      icon: "mingcute:grid-fill",
      tooltip: "Large icons",
    },
    {
      id: "grid:small",
      icon: "gridicons:grid",
      tooltip: "Small icons",
    },
    {
      id: "table",
      icon: "material-symbols:view-list-sharp",
      tooltip: "Details",
    },
  ];
</script>

<LayerContent
  id="content-browser"
  class="flex flex-col w-full items-start justify-start mt-0"
>
  <header class="header">
    <div class="flex items-center justify-start space-x-2 w-full">
      {#each buttons as button}
        <IconButton
          {...button}
          is_active={button.id === current_view_id}
          onclick={() => (current_view_id = button.id)}
        />
      {/each}
    </div>

    <div class="px-2 mb-1 py-3 max-w-full">
      <Input placeholder="Search" class="rounded-sm h-8" />
    </div>
  </header>

  <SplitLayer direction="horizontal">
    <FileTree files={$files} {handleSelectFile} {selected_file} />
    <LayerHandle />
    <FolderContent
      files={$files}
      current_path={$current_path}
      {selected_file}
      {handleSelectFile}
    />
  </SplitLayer>
</LayerContent>

<style lang="postcss">
  .header {
    @apply bg-bunker-1000 w-full flex items-center px-4;
  }
</style>
