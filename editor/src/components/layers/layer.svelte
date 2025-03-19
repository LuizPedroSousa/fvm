<script module lang="ts">
  export interface Pane {
    id: PaneId;
    name: string;
    icon: string;
  }

  export type LayerProps = Partial<PaneProps> & {
    panes?: Pane[];
    default_pane?: PaneId;
  };
</script>

<script lang="ts">
  import * as Tabs from "$components/ui/tabs";

  import { Pane as PaneForge, type PaneProps } from "paneforge";
  import type { PaneId } from "../../contexts/engine.context";

  const { panes, default_pane, ...props }: LayerProps = $props();

  let active_tab = $state(
    default_pane ?? (panes?.length > 0 ? panes[0].id : undefined),
  );

  const changeActiveTab = (value: PaneId) => {
    active_tab = value;
  };
</script>

<PaneForge
  class="bg-bunker-950 flex flex-col w-full h-full relative overflow-scroll-y rounded-r-md"
  {...props}
>
  {#if !!panes && panes?.length > 0}
    <Tabs.Root bind:value={active_tab}>
      <Tabs.PaneHeader {panes} {active_tab} {changeActiveTab} />
      <slot />
    </Tabs.Root>
  {:else}
    <slot />
  {/if}
</PaneForge>
