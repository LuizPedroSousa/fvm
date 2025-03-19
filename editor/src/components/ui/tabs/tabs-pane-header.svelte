<script module lang="ts">
  export interface TabsPaneHeaderProps extends TabsPrimitive.ListProps {
    panes: Pane[];
    active_tab: PaneId;
    changeActiveTab: (value: PaneId) => void;
  }
</script>

<script lang="ts">
  import { Tabs as TabsPrimitive } from "bits-ui";
  import { cn } from "$helpers/styling";
  import type { EngineContext, PaneId } from "../../../contexts/engine.context";
  import { getContext } from "svelte";
  import Icon from "@iconify/svelte";
  import type { Pane } from "$components/layers/layer.svelte";

  const {
    class: className,
    active_tab,
    changeActiveTab,
    panes,
    ...props
  }: TabsPaneHeaderProps = $props();

  const { activePaneId, setActivePaneId } = getContext<EngineContext>("engine");
</script>

<TabsPrimitive.List
  class={cn(
    "flex h-8 w-full flex items-center justify-start bg-bunker-1000 border-b-2 border-bunker-950",
    className,
  )}
  {...props}
>
  {#each panes as pane}
    <TabsPrimitive.Trigger
      value={pane.id}
      on:click={() => {
        setActivePaneId(pane.id);
        changeActiveTab(pane.id);
      }}
      class={cn(
        "flex items-center justify-between w-[175px] h-full px-2 text-gray font-medium text-sm ",
        active_tab === pane.id ? "bg-bunker-500" : "bg-bunker-1000",
        $activePaneId === pane.id && "border-t-2 border-sunset-orange-500",
      )}
    >
      <div class="flex items-center hover:opacity-50 transition">
        <span class="mr-1.5 flex items-center justify-center">
          <Icon icon={pane.icon} width="18" />
        </span>
        {pane.name}
      </div>

      <button class="hover:bg-red-500">
        <Icon icon="material-symbols:close" width="14" />
      </button>
    </TabsPrimitive.Trigger>
  {/each}
</TabsPrimitive.List>
