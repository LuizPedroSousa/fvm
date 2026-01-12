<script lang="ts">
  import type { Component } from "../../../contexts/engine.context";
  import VectorInput from "../../../molecules/vector-input.svelte";

  interface Material extends Component {}

  const { data }: MaterialComponentProps = $props();

  interface MaterialComponentProps {
    data: Material;
  }
</script>

<div class="container space-y-2 pt-4"></div>

<style lang="postcss">
  .container {
    @apply flex items-start flex-col w-full justify-start;
  }
</style>
