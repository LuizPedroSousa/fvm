<script lang="ts">
  import Icon from "@iconify/svelte";
  import * as ContextMenu from "../components/ui/context-menu";

  export let item: any;
</script>

{#if item.items && item.items.length > 0}
  <ContextMenu.Sub>
    <ContextMenu.SubTrigger inset>{item.name}</ContextMenu.SubTrigger>
    <ContextMenu.SubContent class="w-48">
      {#each item.items as sub_item}
        <svelte:self item={sub_item} />
      {/each}
    </ContextMenu.SubContent>
  </ContextMenu.Sub>
{:else if item?.type === "separator"}
  <ContextMenu.Separator />
{:else}
  <ContextMenu.Item>
    <div class="flex items-center">
      <span class="flex items-center justify-center mr-2">
        <Icon icon={item.icon} width={14} />
      </span>
      {item.name}
    </div>

    {#if item?.shortcut}
      <ContextMenu.Shortcut>{item.shortcut}</ContextMenu.Shortcut>
    {/if}
  </ContextMenu.Item>
{/if}
