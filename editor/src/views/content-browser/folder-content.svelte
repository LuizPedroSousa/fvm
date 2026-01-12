<script module lang="ts">
  export interface FolderContentProps {
    files: File[];
    current_path: string;
    selected_file?: File;
    handleSelectFile: (File: File) => void;
  }
</script>

<script lang="ts">
  import Layer, { type LayerProps } from "$components/layers/layer.svelte";
  import type { File } from "./content-browser.svelte";

  import * as Breadcrumb from "$components/ui/breadcrumb";
  import Icon from "@iconify/svelte";
  import { cn } from "$helpers/styling";

  const {
    files,
    current_path,
    selected_file,
    handleSelectFile,
  }: FolderContentProps = $props();

  const layer: LayerProps = {};
</script>

<Layer {...layer}>
  <div class="p-4">
    <Breadcrumb.Root>
      <Breadcrumb.List>
        {#each current_path.split("/") as path, i}
          <Breadcrumb.Item>
            <Breadcrumb.Link href="/">{path}</Breadcrumb.Link>
          </Breadcrumb.Item>

          {#if i > 0 && i < current_path.split("/").length - 1}
            <Breadcrumb.Separator />
          {/if}
        {/each}
      </Breadcrumb.List>
    </Breadcrumb.Root>

    <div class="flex flex-wrap gap-4 mt-6">
      {#each files as file}
        <button
          class={cn("file-node", selected_file?.name === file.name && "active")}
          onclick={() => handleSelectFile(file)}
        >
          <span class="icon">
            <Icon
              icon={file.is_dir ? "material-symbols:folder" : "mdi:file"}
              width="28"
            />
          </span>

          <p class="name">{file.name}</p>
        </button>
      {/each}
    </div>
  </div>
</Layer>

<style lang="postcss">
  .file-node {
    @apply w-40 h-40 bg-bunker-800 rounded-md flex flex-col justify-center items-center hover:bg-bunker-900 transition px-4;
  }

  .active {
    @apply bg-sunset-orange-900 border-2 border-sunset-orange-500 hover:opacity-75 hover:bg-sunset-orange-900;
  }

  .file-node icon {
    @apply flex items-center justify-center;
  }

  .file-node .name {
    @apply mt-4 text-sm text-wrap;
  }
</style>
