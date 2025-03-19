<script module lang="ts">
  interface IconButtonProps extends Partial<HTMLButtonElement> {
    tooltip: string;
    icon: string;
    is_active: boolean;
  }
</script>

<script lang="ts">
  import { cn } from "$helpers/styling";

  import Icon from "@iconify/svelte";

  const { icon, tooltip, is_active, ...props }: IconButtonProps = $props();
</script>

<button class={cn("container", is_active && "active")} {...props}>
  <Icon {icon} />
</button>

<style lang="postcss">
  .container {
    @apply flex items-center justify-center border-2 border-bunker-950 w-8 h-8 rounded-md hover:opacity-75 p-0 m-0;
  }

  .active {
    @apply bg-sunset-orange-950 border-sunset-orange-500;
  }
</style>
