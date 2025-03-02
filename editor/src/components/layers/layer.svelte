<script module lang="ts">
  export type LayerProps = Partial<PaneProps> & {
    title?: string | undefined;
  };
</script>

<script lang="ts">
  import { Pane, type PaneProps } from "paneforge";

  export let title: LayerProps["title"];
</script>

<Pane
  class="bg-cinder-950 flex flex-col w-full h-full relative overflow-hidden"
  {...$$restProps}
>
  {#if title}
    <div class="w-full py-1 pl-1 flex items-start border border-handle">
      <strong class="text-gray font-medium text-sm">
        {title}
      </strong>
    </div>
  {/if}
  <slot />
</Pane>
