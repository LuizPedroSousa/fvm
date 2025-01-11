<script lang="ts" strictEvents>
  import { getContext, onDestroy, onMount } from "svelte";
  import type {
    ClientCallback,
    GatheringContext,
    LayerInitFunction,
  } from "../../contexts/gathering.context";
  import { carefullCallbackSource } from "../../helpers/callback";
  import { getDimensionName } from "../../helpers/sizing";
  import type { Layer } from "../../models/layer";
  import { KEY } from "./split-layer.svelte";

  const {
    onLayerInit,
    clientOnly: clientOnlyContext,
    isHorizontal,
    showFirstSplitter,
    veryFirstLayerKey,
  } = getContext<GatheringContext>(KEY);

  // PROPS
  export let size: number | null = null;
  export let minSize = 0;
  export let maxSize = 100;
  export let snapSize = 0;

  // css class
  let clazz = "";
  export { clazz as class };

  // VARIABLES
  const key = {};

  const gathering = false;
  const { undefinedLayerInitSize } = (
    !gathering ? onLayerInit(key) : {}
  ) as ReturnType<LayerInitFunction>;

  let element: HTMLElement;
  let sz: number = size ?? undefinedLayerInitSize;
  let isSplitterActive = false;

  // CALLBACKS
  let clientCallbacks: ClientCallback | undefined = undefined;

  /**
   * This is an object of callbacks that are safe to be called on browser even when the object `clientCallbacks`
   *  isn't initialized yet (i.e. before `onPaneAdd()`).
   *
   * In the case of the object isn't initialized yet, calling this callbacks will do nothing.
   */

  const carefullClientCallbacks = carefullCallbackSource(() => clientCallbacks);

  const reportGivenSizeChangeSafe = (size: number) => {
    // We put an extra check of `size != sz` here and not in the reactive statement, since we don't want a change
    //  of `sz` to trigger report.
    if (clientCallbacks && size != sz) {
      carefullClientCallbacks?.("reportGivenSizeChange")(size);
    }
  };

  // REACTIVE
  $: {
    if (typeof size === "number") {
      reportGivenSizeChangeSafe(size);
    }
  }

  $: dimension = getDimensionName($isHorizontal);
  $: style = `${dimension}: ${sz}%;`;

  onMount(() => {
    const inst: Layer = {
      index: 0,
      key,
      element: element,
      givenSize: size,
      sz: () => sz,
      setSz: (v) => {
        sz = v;
        if (typeof size === "number" && size !== sz) {
          size = sz;
        }
      },
      min: () => minSize,
      max: () => maxSize,
      snap: () => snapSize,
      setSplitterActive: (isActive: boolean) => {
        isSplitterActive = isActive;
      },
      isReady: false,
    };

    clientCallbacks = clientOnlyContext?.onLayerAdd(inst);
  });

  onDestroy(() => {
    clientOnlyContext?.onLayerRemove(key);
  });
</script>

{#if !gathering}
  <!-- Splitter -->
  {#if $veryFirstLayerKey !== key || $showFirstSplitter}
    <!-- this a11y issue is known, and will be taken care of as part of the a11y feature issue in #11 -->
    <!-- svelte-ignore a11y-click-events-have-key-events -->
    <!-- svelte-ignore a11y-no-static-element-interactions -->
    <div
      class="splitlayers__splitter {isSplitterActive
        ? 'splitlayers__splitter__active'
        : ''}"
      on:mousedown={carefullClientCallbacks?.("onSplitterDown")}
      on:touchstart={carefullClientCallbacks?.("onSplitterDown")}
      on:click={carefullClientCallbacks?.("onSplitterClick")}
      on:dblclick={carefullClientCallbacks?.("onSplitterDblClick")}
    />
  {/if}

  <!-- Pane -->
  <!-- this a11y issue is known, and will be taken care of as part of the a11y feature issue in #11 -->
  <!-- svelte-ignore a11y-click-events-have-key-events -->
  <!-- svelte-ignore a11y-no-static-element-interactions -->
  <div
    class={`splitlayers__layer ${clazz || ""}`}
    bind:this={element}
    on:click={carefullClientCallbacks?.("onLayerClick")}
    {style}
  >
    <slot />
  </div>
{/if}
