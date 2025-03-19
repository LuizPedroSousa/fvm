<script lang="ts">
  import { ContextMenu as ContextMenuPrimitive } from "bits-ui";
  import { cn } from "$helpers/styling";

  type $$Props = ContextMenuPrimitive.SeparatorProps;

  let className: $$Props["class"] = undefined;
  export { className as class };
</script>

<ContextMenuPrimitive.Separator
  class={cn("bg-sunset-orange-600 -mx-1 my-1 h-px", className)}
  {...$$restProps}
/>
