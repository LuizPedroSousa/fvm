<script lang="ts">
  import * as Menubar from "../components/ui/menubar";

  export let item: any;
</script>

{#if item.items && item.items.length > 0}
  <Menubar.Sub>
    <Menubar.SubTrigger>{item.name}</Menubar.SubTrigger>
    <Menubar.SubContent>
      {#each item.items as sub_item}
        <svelte:self item={sub_item} />
      {/each}
    </Menubar.SubContent>
  </Menubar.Sub>
{:else}
  <Menubar.Item>
    {item.name}
    {#if item.shortcut}
      <Menubar.Shortcut>⌘{item.shortcut}</Menubar.Shortcut>
    {/if}
  </Menubar.Item>
{/if}
