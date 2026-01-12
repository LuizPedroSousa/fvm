<script module lang="ts">
  export interface LayerContentProps extends TabsPrimitive.ContentProps {
    id: PaneId;
  }
</script>

<script lang="ts">
  import * as Tabs from "$components/ui/tabs";
  import type { PaneId } from "../../contexts/engine.context";

  import { Tabs as TabsPrimitive } from "bits-ui";

  const { id, ...props } = $props();
</script>

<Tabs.Content value={id} class="w-full" {...props}>
  <slot />
</Tabs.Content>
