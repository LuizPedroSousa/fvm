<script lang="ts">
  import { Input } from "$components/ui/input";
  import type { Component } from "../../../contexts/engine.context";
  import type { Vector3 } from "../../../models/math";
  import VectorInput from "../../../molecules/vector-input.svelte";

  interface Transform extends Component {
    position: Vector3;
    rotation: Vector3;
    scale: Vector3;
  }

  const { data }: TransformComponentProps = $props();

  interface TransformComponentProps {
    data: Transform;
  }
</script>

<div class="container space-y-2 py-4">
  <VectorInput name="Position" vector={data?.position} />
  <VectorInput name="Rotation" vector={data?.rotation} />
  <VectorInput name="Scale" vector={data?.scale} />
</div>

<style lang="postcss">
  .container {
    @apply flex px-4 items-start flex-col w-full justify-start;
  }
</style>
