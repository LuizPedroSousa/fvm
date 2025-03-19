<script lang="ts">
  import { Menubar as MenubarPrimitive } from "bits-ui";
  import { cn } from "$helpers/styling";

  type $$Props = MenubarPrimitive.Props;

  let className: $$Props["class"] = undefined;
  export { className as class };
</script>

<MenubarPrimitive.Root
  class={cn(
    "bg-background flex h-8 items-center space-x-1 border p-1",
    className,
  )}
  {...$$restProps}
>
  <slot />
</MenubarPrimitive.Root>
