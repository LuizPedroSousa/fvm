<script lang="ts">
  import type { HTMLThAttributes } from "svelte/elements";
  import { cn } from "$helpers/styling";

  type $$Props = HTMLThAttributes;

  let className: $$Props["class"] = undefined;
  export { className as class };
</script>

<th
  class={cn(
    "text-muted-foreground h-12 px-4 text-left align-middle font-medium [&:has([role=checkbox])]:pr-0",
    className,
  )}
  {...$$restProps}
>
  <div class="resizer" on:click|stopPropagation />
  <slot />
</th>

<style>
  th {
    position: relative;
  }

  .resizer {
    position: absolute;
    top: 0;
    bottom: 0;
    right: -4px;
    width: 2px;
    background: lightgray;
    cursor: col-resize;
    z-index: 1;
  }
</style>
