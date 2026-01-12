<script lang="ts">
  import "../app.css";
  import "@fontsource/roboto";
  import Engine from "../contexts/engine.svelte";

  let { children } = $props();
</script>

{@render children()}
