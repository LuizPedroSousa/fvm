<script lang="ts">
  import { Input } from "$components/ui/input";
  import type { Component } from "../../../contexts/engine.context";
  import type { Vector3 } from "../../../models/math";
  import VectorInput from "../../../molecules/vector-input.svelte";

  interface RigidBody extends Component {}

  const { data }: RigidBodyComponentProps = $props();

  interface RigidBodyComponentProps {
    data: RigidBody;
  }
</script>

<div class="container space-y-2 py-4"></div>

<style lang="postcss">
  .container {
    @apply flex px-4 items-start flex-col w-full justify-start;
  }
</style>
