<script lang="ts">
  import { ContextMenu as ContextMenuPrimitive } from "bits-ui";
  import { cn, flyAndScale } from "$helpers/styling";

  type $$Props = ContextMenuPrimitive.SubContentProps;

  let className: $$Props["class"] = undefined;
  export let transition: $$Props["transition"] = flyAndScale;
  export let transitionConfig: $$Props["transitionConfig"] = {
    x: -10,
    y: 0,
  };
  export { className as class };
</script>

<ContextMenuPrimitive.SubContent
  {transition}
  {transitionConfig}
  class={cn(
    "bg-bunker-950 text-popover-foreground rounded-md border border-sunset-orange-600 p-1 shadow-md focus:outline-none",
    className,
  )}
  {...$$restProps}
  on:keydown
  on:focusout
  on:pointermove
>
  <slot />
</ContextMenuPrimitive.SubContent>
