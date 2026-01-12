<script lang="ts">
  import { ContextMenu as ContextMenuPrimitive } from "bits-ui";
  import { cn, flyAndScale } from "$helpers/styling";

  type $$Props = ContextMenuPrimitive.ContentProps;

  let className: $$Props["class"] = undefined;
  export let transition: $$Props["transition"] = flyAndScale;
  export let transitionConfig: $$Props["transitionConfig"] = undefined;
  export { className as class };
</script>

<ContextMenuPrimitive.Content
  {transition}
  {transitionConfig}
  class={cn(
    "bg-bunker-950 text-popover-foreground z-50 border-sunset-orange-600 rounded-md border py-2 px-1 shadow-md focus:outline-none",
    className,
  )}
  {...$$restProps}
  on:keydown
>
  <slot />
</ContextMenuPrimitive.Content>
