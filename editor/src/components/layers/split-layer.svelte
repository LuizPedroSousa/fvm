<script context="module" lang="ts">
  export const KEY = {};

  type ResultType = boolean | { passive: boolean };
  const thirdEventArg = (() => {
    let result: ResultType = false;

    try {
      const arg = Object.defineProperty({}, "passive", {
        get() {
          result = { passive: true };
          return true;
        },
      });

      // @ts-expect-error no overload matches
      window.addEventListener("testpassive", arg, arg);

      // @ts-expect-error no overload matches
      window.remove("testpassive", arg, arg);
    } catch (_e) {
      /* */
    }

    return result;
  })();
</script>

<script lang="ts" strictEvents>
  import {
    onMount,
    onDestroy,
    setContext,
    createEventDispatcher,
    tick,
    afterUpdate,
  } from "svelte";
  import { writable } from "svelte/store";
  import type { Layer, LayerSizingEvent } from "../../models/layer";
  import type {
    ClientCallback,
    GatheringContext,
    LayerInitFunction,
  } from "../../contexts/gathering.context";
  import GatheringContextProvider from "../../contexts/gathering-provider.svelte";
  import { calcComputedStyle } from "../../helpers/styling";
  import {
    elementRectWithoutBorder,
    getElementRect,
    getGlobalMousePosition,
    positionDiff,
    type Position,
  } from "../../helpers/position";
  import { getDimensionName } from "../../helpers/sizing";
  import { forEachPartial, sumPartial } from "../../helpers/array";
  import GatheringProvider from "../../contexts/gathering-provider.svelte";

  // eslint-disable-next-line
  interface $$Events {
    "layer-click": CustomEvent<Layer>;
    "splitter-click": CustomEvent<Layer>;
    "layer-maximize": CustomEvent<Layer>;
    ready: CustomEvent<void>;
    resize: CustomEvent<LayerSizingEvent[]>;
    resized: CustomEvent<LayerSizingEvent[]>;
    "layer-add": CustomEvent<{
      index: number;
      layers: LayerSizingEvent[];
    }>;

    "layer-remove": CustomEvent<{
      removed: Layer;
      layers: LayerSizingEvent[];
    }>;
  }

  interface Sums {
    prevLayersSize: number;
    nextLayersSize: number;
    prevReachedMinLayers: number;
    nextReachedMinLayers: number;
  }

  //@ts-expect-error undefined not assigned to string
  export let id: string = undefined;
  // horiz or verti?
  export let horizontal = false;
  // when true, moving a splitter can push other panes
  export let pushOtherLayers = true;
  // open/close on double click
  export let dblClickSplitter = true;
  // true if RTL
  export let rtl: boolean | "auto" = "auto";
  // true to display the first splitter
  export let firstSplitter = false;
  // css style
  export let style: string | null = null;
  // the splitter theme to use
  export let theme = "default-theme";
  // css class
  let clazz = "";
  export { clazz as class };

  // VARIABLES ----------------

  //used to bubble events up
  const dispatch = createEventDispatcher();

  // the splitpane component
  let container: HTMLElement;
  // true when component is ready, prevents emitting console warnings on hot reloading.
  let isReady = false;
  // true when pane reset is awaiting for the next tick, to avoid double call to pane reset.
  let isAwaitingLayerReset = false;
  // true after the initial timeout 0 waiting, prevents CSS transitions until then.
  let isAfterInitialTimeoutZero = false;
  // true when mouse is down
  let isMouseDown = false;
  // true when a splitter is being dragged
  let isDragging = false;
  // that's the splitter than is being dragged
  let activeSplitter = -1;
  // that's well the clicked splitter!
  let clickedSplitter = -1;
  // used to detect double clicks
  let timeoutId: number | null;
  // panes per insertion order (pane.index is the order index)
  let layers = new Array<Layer>();
  // passed to the children via the context - writable to ensure proper reactivity
  const isHorizontal = writable<boolean>(horizontal);
  const showFirstSplitter = writable<boolean>(firstSplitter);
  // tells the key of the very first pane, or undefined if not recieved yet
  const veryFirstLayerKey = writable<unknown>(undefined);
  let activeSplitterElement: HTMLElement;
  let activeSplitterDrag: number;

  // REACTIVE ----------------

  $: $isHorizontal = horizontal;
  $: $showFirstSplitter = firstSplitter;

  const onLayerInit: LayerInitFunction = (_key) => {
    if ($veryFirstLayerKey === undefined) {
      $veryFirstLayerKey = _key;
    }

    return {
      undefinedLayerInitSize: 0,
    };
  };

  setContext<GatheringContext>(KEY, {
    showFirstSplitter,
    veryFirstLayerKey,
    isHorizontal,
    onLayerInit,
    clientOnly: {
      onLayerAdd,
      onLayerRemove,
    },
  });

  function onLayerAdd(pane: Layer): ClientCallback {
    // 1. Add pane to array at the same index it was inserted in the <splitlayers> tag.
    let index = -1;
    if (pane.element.parentNode) {
      Array.from(pane.element.parentNode.children).some((el: Element) => {
        if (el.className.includes("splitlayers__layer")) index++;
        return el === pane.element;
      });
    }

    if (index === 0) {
      // Need to update the first pane key, because the first pane can be changed in runtime.
      $veryFirstLayerKey = pane.key;
    }

    //inserts pane at proper array index
    layers.splice(index, 0, pane);

    // reindex panes
    for (let i = 0; i < layers.length; i++) {
      layers[i].index = i;
    }

    if (isReady) {
      // 2. tick and resize the panes.
      tickAndResetLayerSizes().then(() => {
        // 3. Set the pane as ready
        pane.isReady = true;

        // 4. Fire `pane-add` event.
        dispatch("layer-add", {
          index,
          layers: prepareSizeEvent(),
        });
      });
    }

    const paneForward =
      <T,>(cb: (value: T, pane: Layer) => void, includingFirst = true) =>
      (value: T) => {
        if (includingFirst || pane.index > 0) {
          cb(value, pane);
        }
      };

    return {
      onSplitterDown: paneForward(onMouseDown, false),
      onSplitterClick: paneForward(onSplitterClick, false),
      onSplitterDblClick: paneForward(onSplitterDblClick),
      onLayerClick: paneForward(onLayerClick),
      reportGivenSizeChange: paneForward(reportGivenSizeChange),
    };
  }

  async function onLayerRemove(key: unknown) {
    // 1. Remove the pane from array and redo indexes.
    const index = layers.findIndex((p) => p.key === key);

    // race condition - typically happens when the dev server restarts
    if (index >= 0) {
      const removed = layers.splice(index, 1)[0];

      // reindex panes
      for (let i = 0; i < layers.length; i++) {
        layers[i].index = i;
      }

      if (index === 0) {
        $veryFirstLayerKey = layers.length > 0 ? layers[0].key : undefined;
      }

      if (isReady) {
        // 3. tick and resize the panes.
        await tickAndResetLayerSizes();

        // 4. Fire `pane-remove` event.
        dispatch("layer-remove", {
          removed,
          layers: prepareSizeEvent(),
        });
      }
    }
  }

  // called by sub-panes
  function onLayerClick(_event: MouseEvent, pane: Layer) {
    dispatch("layer-click", pane);
  }

  function reportGivenSizeChange(newGivenSize: number, pane: Layer) {
    pane.setSz(newGivenSize);
    tickAndResetLayerSizes();
  }

  onMount(() => {
    verifyAndUpdateLayersOrder();
    resetLayerSizes();

    for (let i = 0; i < layers.length; i++) {
      layers[i].isReady = true;
    }
    isReady = true;
    dispatch("ready");

    setTimeout(() => {
      isAfterInitialTimeoutZero = true;
    }, 0);
  });

  onDestroy(() => {
    if (isReady) {
      // this is to solve an edge case:
      // when the user starts dragging and the component is destroyed, leaving behind hanging events
      unbindEvents();
    }

    // Prevent emitting console warnings on hot reloading.
    isReady = false;
  });

  afterUpdate(() => {
    verifyAndUpdateLayersOrder();
  });

  // Tells in the current DOM state if we are in RTL direction or not.
  function isRTL(containerComputedStyle?: CSSStyleDeclaration) {
    if (rtl === "auto") {
      // the try catch is to support old browser, flag is preset to false
      try {
        return (
          (containerComputedStyle ?? calcComputedStyle(container)).direction ===
          "rtl"
        );
      } catch (_err) {
        // We want application to not crush, but don't care about the message
      }
    }
    // otherwise

    return rtl === true;
  }

  function bindEvents() {
    document.body.style.cursor = isHorizontal ? "col-resize" : "row-resize";

    document.addEventListener("mousemove", onMouseMove, thirdEventArg);
    document.addEventListener("mouseup", onMouseUp);

    if ("ontouchstart" in window) {
      document.addEventListener("touchmove", onMouseMove, thirdEventArg);
      document.addEventListener("touchend", onMouseUp);
    }
  }

  function unbindEvents() {
    document.body.style.cursor = "";

    document.removeEventListener("mousemove", onMouseMove);
    document.removeEventListener("mouseup", onMouseUp);

    if ("ontouchstart" in window) {
      document.removeEventListener("touchmove", onMouseMove);
      document.removeEventListener("touchend", onMouseUp);
    }
  }

  const isSplitterElement = (node: Node) =>
    node.nodeType === Node.ELEMENT_NODE &&
    (node as HTMLElement).classList.contains("splitlayers__splitter");

  function getOrientedDiff(
    drag: Position,
    elementSize: number,
    isRTL: boolean
  ): number {
    let tdrag = drag[horizontal ? "top" : "left"];
    if (isRTL && !horizontal) tdrag = elementSize - tdrag;

    return tdrag;
  }

  const getCurrentDimensionName = () => getDimensionName(horizontal);

  function onMouseDown(event: TouchEvent | MouseEvent, splitterLayer: Layer) {
    isMouseDown = true;
    activeSplitter = splitterLayer.index;

    splitterLayer.setSplitterActive(true);
    const paneElement = splitterLayer.element;

    let activeSplitterNode: Node | null = paneElement;
    while (activeSplitterNode) {
      activeSplitterNode = activeSplitterNode.previousSibling;
      if (activeSplitterNode && isSplitterElement(activeSplitterNode)) {
        break;
      }
    }
    if (activeSplitterNode === null) {
      console.error("Splitpane Error: Active splitter wasn't found!");
      return; // Don't bind move event on error
    }

    activeSplitterElement = activeSplitterNode as HTMLElement;

    const globalMousePosition = getGlobalMousePosition(event);
    const splitterRect = getElementRect(activeSplitterElement);
    activeSplitterDrag = getOrientedDiff(
      positionDiff(globalMousePosition, splitterRect),
      splitterRect[getCurrentDimensionName()],
      isRTL()
    );

    bindEvents();
  }

  function onMouseMove(event: MouseEvent | TouchEvent) {
    if (isMouseDown) {
      isDragging = true;

      const globalMousePosition = getGlobalMousePosition(event);
      const containerComputedStyle = calcComputedStyle(container);
      const containerRectWithoutBorder = elementRectWithoutBorder(
        container,
        containerComputedStyle
      );
      const containerSizeWithoutBorder: number =
        containerRectWithoutBorder[getCurrentDimensionName()];
      const _isRTL = isRTL(containerComputedStyle);

      const currentMouseDrag = positionDiff(
        globalMousePosition,
        containerRectWithoutBorder
      );
      const tdrag = getOrientedDiff(
        currentMouseDrag,
        containerSizeWithoutBorder,
        _isRTL
      );
      calculateLayersSize(tdrag, containerSizeWithoutBorder);

      dispatch("resize", prepareSizeEvent());
    }
  }

  function onMouseUp() {
    if (isDragging) {
      dispatch("resized", prepareSizeEvent());
    }
    isMouseDown = false;

    const pane = layers[activeSplitter];
    pane.setSplitterActive(false);

    // Keep dragging flag until click event is finished (click happens immediately after mouseup)
    // in order to prevent emitting `splitter-click` event if splitter was dragged.
    setTimeout(() => {
      isDragging = false;
      unbindEvents();
    }, 100);
  }

  // If touch device, detect double tap manually (2 taps separated by less than 500ms).
  function onSplitterClick(event: MouseEvent, splitterLayer: Layer) {
    if ("ontouchstart" in window) {
      const splitterIndex = splitterLayer.index;

      // Detect splitter double taps if the option is on.
      if (dblClickSplitter) {
        if (clickedSplitter === splitterIndex) {
          if (timeoutId) clearTimeout(timeoutId);
          timeoutId = null;
          onSplitterDblClick(event, splitterLayer);
          clickedSplitter = -1; // Reset for the next tap check.
        } else {
          clickedSplitter = splitterIndex;
          timeoutId = setTimeout(() => {
            clickedSplitter = -1;
          }, 500);
        }
      }
    }

    if (!isDragging) dispatch("splitter-click", splitterLayer);
  }

  // On splitter dbl click or dbl tap maximize this pane.
  function onSplitterDblClick(_event: MouseEvent, splitterLayer: Layer) {
    if (dblClickSplitter) {
      const splitterIndex = splitterLayer.index;

      let totalMinSizes = 0;
      for (let i = 0; i < layers.length; i++) {
        const pane = layers[i];
        if (i !== splitterIndex) {
          totalMinSizes += pane.min();
        }
      }

      const maxExtendedSize = Math.min(
        Math.max(0, 100 - totalMinSizes),
        splitterLayer.max()
      );

      const totalMaxExtendedPlusMinSizes = totalMinSizes + maxExtendedSize;
      if (totalMaxExtendedPlusMinSizes >= 100) {
        // put everything to the minimum, and in the splitterLayer put the rest of the size
        for (let i = 0; i < layers.length; i++) {
          const pane = layers[i];
          if (pane !== splitterLayer) {
            pane.setSz(pane.min());
          } else {
            pane.setSz(100 - totalMinSizes);
          }
        }
      } else {
        // notice that in this case, we can conclude that `panes.length >= 2`

        // put splitterLayer to the maximum (the actual one), and the normal panes to the minimum,
        //  and give the spare to left pane (or to the right pane, if the splitterLayer is the first pane)
        // if this spare size is beyond the pane maximum, need to pass it along to the other panes

        let leftSpare = 100 - totalMaxExtendedPlusMinSizes;

        splitterLayer.setSz(maxExtendedSize);

        const giveBest = (pane: Layer) => {
          const min = pane.min();
          const max = pane.max();

          const szExtra = Math.min(Math.max(0, leftSpare), max - min);
          pane.setSz(min + szExtra);
          leftSpare -= szExtra;
        };

        // go backward and give the most size as we can
        for (let i = splitterIndex - 1; i >= 0; i--) giveBest(layers[i]);

        // go forward and give the most size as we can
        for (let i = splitterIndex + 1; i < layers.length; i++)
          giveBest(layers[i]);

        // at the end of the process, we must have that `leftSpare` is 0
        if (leftSpare !== 0) {
          console.warn(
            "Splitpanes: there is a left spare size after computation of splitter double click, which means there are issues on the size constains of the panes."
          );
        }
      }

      dispatch("layer-maximize", splitterLayer);
      dispatch("resized", prepareSizeEvent());
    }
    // onMouseUp might not be called on the second click, so update the mouse state.
    // TODO: Should also check and unbind events, but better IMO to not bind&unbind on every click, so ignored for now.
    isMouseDown = false;
  }

  const prepareSizeEvent = (): LayerSizingEvent[] =>
    layers.map((pane) => ({
      min: pane.min(),
      max: pane.max(),
      size: pane.sz(),
      snap: pane.snap(),
    }));

  /**
   * Returns the drag percentage of the splitter relative to the 2 parts it's inbetween, meaning the ratio between
   *  the size that all the panes before the splitter consumes (ignoring other splitters size) and the total size of the container.
   */
  function getCurrentDragPercentage(
    tdrag: number,
    containerSizeWithoutBorder: number
  ) {
    // Here we want the splitter size **including the borders**.
    // We need to use `Element.getBoundingClientRect()` and not `Element.clientWidth` and `Element.clientHeight`,
    //  bacause the latter round the number of pixels to integer, and additionally, they don't include the borders.
    const splitterSize = (node: Node) =>
      getElementRect(node as HTMLElement)[getCurrentDimensionName()];

    const activeSplitterSize = splitterSize(activeSplitterElement);

    let splittersTotalSizeBefore = 0;
    let currentBeforeNode = activeSplitterElement.previousSibling;
    while (currentBeforeNode) {
      if (isSplitterElement(currentBeforeNode)) {
        splittersTotalSizeBefore += splitterSize(currentBeforeNode);
      }
      currentBeforeNode = currentBeforeNode.previousSibling;
    }

    let splittersTotalSizeAfter = 0;
    let currentAfterNode = activeSplitterElement.nextSibling;
    while (currentAfterNode) {
      if (isSplitterElement(currentAfterNode)) {
        splittersTotalSizeAfter += splitterSize(currentAfterNode);
      }
      currentAfterNode = currentAfterNode.nextSibling;
    }

    const totalSplitterBefore = splittersTotalSizeBefore + activeSplitterDrag;
    const totalSplitter =
      splittersTotalSizeBefore + activeSplitterSize + splittersTotalSizeAfter;

    // An explanation to the mathematical computation:
    //
    // Let's start with the case of only two panes. If we mark the first pane size in prec
    //  (thinking about it as a number between 0 to 1) as `x`, we'll get that the size of the left pane in pixels will be:
    // `x*containerSizeWithoutBorder - x*totalSplitter = x*(containerSizeWithoutBorder - totalSplitter)`
    // Since we want that the total size in pixels before the user mouse pointer will be `tdrag`, and we need to add the
    //  size of the splitter itself that is before the mouse pointer, we get the equation:
    // `x*(containerSizeWithoutBorder - totalSplitter) + activeSplitterDrag = tdrag`
    //
    // Now in the general case when we have many panes before the splitter, mark their precentages
    //  (again, thinking about it as a number between 0 to 1) by x1,x2,...,xn we'll get the equation:
    // `(x1 + ... + xn)*(containerSizeWithoutBorder - totalSplitter) + totalSplitterBefore = tdrag`
    // And solving it yeild the answer:
    // `x1 + ... + xn = (tdrag - totalSplitterBefore) / (containerSizeWithoutBorder - totalSplitter)`

    return (
      ((tdrag - totalSplitterBefore) /
        (containerSizeWithoutBorder - totalSplitter)) *
      100
    );
  }

  /**
   * Called when slitters are moving to adjust pane sizes
   */
  function calculateLayersSize(
    tdrag: number,
    containerSizeWithoutBorder: number
  ) {
    let paneBeforeIndex = activeSplitter - 1;
    let paneBefore = layers[paneBeforeIndex];

    let paneAfterIndex = activeSplitter;
    let paneAfter = layers[paneAfterIndex];

    let sums: Sums = {
      prevLayersSize: sumPrevLayersSize(paneBeforeIndex),
      nextLayersSize: sumNextLayersSize(paneAfterIndex),
      prevReachedMinLayers: 0,
      nextReachedMinLayers: 0,
    };

    // If not pushing other panes, panes to resize are right before and right after splitter.
    const minDrag = 0 + (pushOtherLayers ? 0 : sums.prevLayersSize);
    const maxDrag = 100 - (pushOtherLayers ? 0 : sums.nextLayersSize);

    // Calculate drag percentage
    const mouseDragPercentage = Math.max(
      Math.min(
        getCurrentDragPercentage(tdrag, containerSizeWithoutBorder),
        maxDrag
      ),
      minDrag
    );

    // Handle snap
    const paneBeforeSnap =
      sums.prevLayersSize + paneBefore.min() + paneBefore.snap();

    const paneAfterSnap =
      100 - (sums.nextLayersSize + paneAfter.min() + paneAfter.snap());

    let dragPercentage = mouseDragPercentage;
    let snapped = false;

    if (mouseDragPercentage <= paneBeforeSnap) {
      if (mouseDragPercentage > sums.prevLayersSize + paneBefore.min()) {
        dragPercentage = Math.max(
          paneBefore.min() + sums.prevLayersSize,
          100 - (paneAfter.max() + sums.nextLayersSize)
        );
        snapped = true;
      }
    } else if (mouseDragPercentage >= paneAfterSnap) {
      if (mouseDragPercentage < 100 - sums.nextLayersSize - paneAfter.min()) {
        dragPercentage = Math.min(
          100 - (paneAfter.min() + sums.nextLayersSize),
          paneBefore.max() + sums.prevLayersSize
        );
        snapped = true;
      }
    }

    const paneBeforeMaxReached =
      paneBefore.max() < 100 &&
      dragPercentage >= paneBefore.max() + sums.prevLayersSize;
    const paneAfterMaxReached =
      paneAfter.max() < 100 &&
      dragPercentage <= 100 - (paneAfter.max() + sums.nextLayersSize);
    // Prevent dragging beyond pane max.
    if (paneBeforeMaxReached || paneAfterMaxReached) {
      if (paneBeforeMaxReached) {
        paneBefore.setSz(paneBefore.max());
        paneAfter.setSz(
          Math.max(
            100 - paneBefore.max() - sums.prevLayersSize - sums.nextLayersSize,
            0
          )
        );
      } else {
        paneBefore.setSz(
          Math.max(
            100 - paneAfter.max() - sums.prevLayersSize - sums.nextLayersSize,
            0
          )
        );
        paneAfter.setSz(paneAfter.max());
      }
    } else {
      // When pushOtherLayers = true, find the closest expanded pane on each side of the splitter.
      // TODO: Bug: This should work also when removing `!snapped` condition, but it's not!
      //   To reproduce, reload the example page and see the example "Min & max with snap".
      //   It gets wrongly pushed when try to snap on the initial dragging of the first splitter to the right.
      if (pushOtherLayers && !snapped) {
        const vars = doPushOtherLayers(sums, dragPercentage);
        if (!vars) {
          //		setAllLayerDimensions();
          return; // Prevent other calculation.
        }
        ({ sums, paneBeforeIndex, paneAfterIndex } = vars);
        paneBefore = layers[paneBeforeIndex];
        paneAfter = layers[paneAfterIndex];
      }

      if (typeof paneBeforeIndex === "number") {
        paneBefore.setSz(
          Math.min(
            Math.max(
              dragPercentage - sums.prevLayersSize - sums.prevReachedMinLayers,
              paneBefore.min()
            ),
            paneBefore.max()
          )
        );
      }
      if (typeof paneAfterIndex === "number") {
        paneAfter.setSz(
          Math.min(
            Math.max(
              100 -
                dragPercentage -
                sums.nextLayersSize -
                sums.nextReachedMinLayers,
              paneAfter.min()
            ),
            paneAfter.max()
          )
        );
      }
    }
  }

  function doPushOtherLayers(sums: Sums, dragPercentage: number) {
    const splitterIndex = activeSplitter - 1;
    let layerBeforeIndex: number | undefined = splitterIndex;
    let layerAfterIndex: number | undefined = splitterIndex + 1;
    // Pushing Down.
    // Going smaller than the current pane min size: take the previous expanded pane.
    if (dragPercentage < sums.prevLayersSize + layers[layerBeforeIndex].min()) {
      layerBeforeIndex = findPrevExpandedLayer(splitterIndex)?.index;

      sums.prevReachedMinLayers = 0;

      if (layerBeforeIndex === undefined) {
        // If nothing else to push down, cancel dragging.
        sums.prevLayersSize = 0;
        layers[0].setSz(layers[0].min());
        forEachPartial(layers, 1, splitterIndex + 1, (pane: Layer) => {
          pane.setSz(pane.min());
          sums.prevReachedMinLayers += pane.min();
        });

        layers[layerAfterIndex].setSz(
          100 -
            sums.prevReachedMinLayers -
            layers[0].min() -
            sums.prevLayersSize -
            sums.nextLayersSize
        );
        return null;
      } else {
        // If pushing a n-2 or less pane, from splitter, then make sure all in between is at min size.
        if (layerBeforeIndex < splitterIndex) {
          forEachPartial(
            layers,
            layerBeforeIndex + 1,
            splitterIndex + 1,
            (pane: Layer) => {
              pane.setSz(pane.min());
              sums.prevReachedMinLayers += pane.min();
            }
          );
        }
        sums.prevLayersSize = sumPrevLayersSize(layerBeforeIndex);
      }
    }
    // Pushing Up.
    // Pushing up beyond min size is reached: take the next expanded pane.
    if (
      dragPercentage >
      100 - sums.nextLayersSize - layers[layerAfterIndex].min()
    ) {
      layerAfterIndex = findNextExpandedLayer(splitterIndex)?.index;
      if (layerBeforeIndex === undefined)
        console.log("Yep undefined paneAfterIndex");
      sums.nextReachedMinLayers = 0;

      if (layerAfterIndex === undefined) {
        // If nothing else to push up, cancel dragging.
        sums.nextLayersSize = 0;

        const panesCount = layers.length;
        layers[panesCount - 1].setSz(layers[panesCount - 1].min());

        forEachPartial(layers, splitterIndex + 1, panesCount - 1, (pane) => {
          pane.setSz(pane.min());
          sums.nextReachedMinLayers += pane.min();
        });

        layers[layerBeforeIndex].setSz(
          100 -
            sums.prevLayersSize -
            sums.nextReachedMinLayers -
            layers[panesCount - 1].min() -
            sums.nextLayersSize
        );
        return null;
      } else {
        // If pushing a n+2 or more pane, from splitter, then make sure all in between is at min size.
        if (layerAfterIndex > splitterIndex + 1) {
          forEachPartial(
            layers,
            splitterIndex + 1,
            layerAfterIndex,
            (pane: Layer) => {
              pane.setSz(pane.min());
              sums.nextReachedMinLayers += pane.min();
            }
          );
        }
        sums.nextLayersSize = sumNextLayersSize(layerAfterIndex);
      }
    }
    return {
      sums,
      paneBeforeIndex: layerBeforeIndex,
      paneAfterIndex: layerAfterIndex,
    };
  }

  const getSizeOfLayer = (pane: Layer) => pane.sz();
  const sumPrevLayersSize = (splitterIndex: number) =>
    sumPartial(layers, 0, splitterIndex, getSizeOfLayer);
  const sumNextLayersSize = (splitterIndex: number) =>
    sumPartial(layers, splitterIndex + 1, layers.length, getSizeOfLayer);

  // Return the previous pane from siblings which has a size (width for vert or height for horz) of more than 0.
  const findPrevExpandedLayer = (splitterIndex: number): Layer | undefined =>
    [...layers]
      .reverse()
      .find((p) => p.index < splitterIndex && p.sz() > p.min());

  // Return the next pane from siblings which has a size (width for vert or height for horz) of more than 0.
  const findNextExpandedLayer = (splitterIndex: number): Layer | undefined =>
    layers.find((p) => p.index > splitterIndex + 1 && p.sz() > p.min());

  async function tickAndResetLayerSizes() {
    isAwaitingLayerReset = true;
    await tick();

    if (isAwaitingLayerReset) {
      resetLayerSizes();
      isAwaitingLayerReset = false;
    }
  }

  /**
   *
   * @param addedLayer
   * @param removedLayer
   */
  function resetLayerSizes() {
    equalize();

    if (isReady) dispatch("resized", prepareSizeEvent());
  }

  function equalize() {
    // Escape the function on the edge case that there is not even a single pane
    if (layers.length === 0) {
      return;
    }
    // otherwise

    const panesCount = layers.length;
    let leftToAllocate = 100;
    let definedSizesCount = 0;
    let undefinedSizesNotReadyCount = 0;
    let undefinedSizesSum = 0;
    const ungrowable: Layer[] = [];
    const unshrinkable: Layer[] = [];

    for (let i = 0; i < panesCount; i++) {
      const pane = layers[i];
      const sz = pane.sz();
      if (pane.givenSize === null) {
        if (pane.isReady) {
          undefinedSizesSum += sz;
          if (sz >= pane.max()) ungrowable.push(pane);
          if (sz <= pane.min()) unshrinkable.push(pane);
        } else {
          undefinedSizesNotReadyCount += 1;
        }
      } else {
        // if the size is defined, we don't modify its size at all
        leftToAllocate -= sz;
        definedSizesCount++;
        ungrowable.push(pane);
        unshrinkable.push(pane);
      }
    }

    const undefinedSizesCount = panesCount - definedSizesCount;
    const undefinedSizesReadyCount =
      undefinedSizesCount - undefinedSizesNotReadyCount;

    // the proportion of the newly added panes
    let undefinedSizesNotReadySz: number;
    let undefinedScaleFactor: number;
    if (undefinedSizesReadyCount > 0) {
      // if has undefined sizes panes that are ready:
      undefinedSizesNotReadySz = undefinedSizesSum / undefinedSizesReadyCount;
      if (undefinedSizesNotReadySz > 0.1 && leftToAllocate > 0.1) {
        undefinedSizesSum +=
          undefinedSizesNotReadyCount * undefinedSizesNotReadySz;
        undefinedScaleFactor = leftToAllocate / undefinedSizesSum;
      } else {
        // when the size of the ready undefined panes shares are negligible, need to set the not ready
        //  undefined one to size 0, for being "proportional" to negligible sizes
        undefinedSizesNotReadySz = 0;
        undefinedScaleFactor = 1;
      }
    } else {
      // otherwise, divide the space of the undefined sizes panes equally:
      undefinedSizesNotReadySz = leftToAllocate / undefinedSizesCount;
      undefinedScaleFactor = 1;
    }

    // whenever `leftToAllocate` or `undefinedSizesSum` aren't negligible, need to adjact the sizes
    if (leftToAllocate + undefinedSizesSum > 0.1) {
      leftToAllocate = 100; // reset the space calculation

      for (let i = 0; i < panesCount; i++) {
        const pane = layers[i];
        if (!(typeof pane.givenSize === "number")) {
          // add the proportion of the newly added pane if has undefined size
          const currentSz = pane.isReady ? pane.sz() : undefinedSizesNotReadySz;
          const sz = Math.max(
            Math.min(currentSz * undefinedScaleFactor, pane.max()),
            pane.min()
          );
          pane.setSz(sz);
        }
        leftToAllocate -= pane.sz();
      }

      // since we multiply by scaling, there might be left space that is needed to be saturated
      if (Math.abs(leftToAllocate) > 0.1) {
        leftToAllocate = readjustSizes(
          leftToAllocate,
          ungrowable,
          unshrinkable
        );
      }
    }

    if (!isFinite(leftToAllocate)) {
      console.warn(
        "Splitpanes: Internal error, sizes might be NaN as a result."
      );
    } else if (Math.abs(leftToAllocate) > 0.1) {
      console.warn(
        "Splitpanes: Could not resize panes correctly due to their constraints."
      );
    }
  }

  // Second loop to adjust sizes now that we know more about the panes constraints.
  function readjustSizes(
    leftToAllocate: number,
    ungrowable: Array<Layer>,
    unshrinkable: Array<Layer>
  ): number {
    const panesCount = layers.length;
    const panesSizableCount =
      panesCount -
      (leftToAllocate > 0 ? ungrowable.length : unshrinkable.length);
    if (panesSizableCount <= 0) {
      return leftToAllocate;
    }

    const equalSpaceToAllocate = leftToAllocate / panesSizableCount;

    if (layers.length === 1) {
      layers[0].setSz(100);
      leftToAllocate = 0;
    } else
      for (let i = 0; i < layers.length; i++) {
        const pane = layers[i];
        const sz = pane.sz();
        if (leftToAllocate > 0 && !ungrowable.includes(pane)) {
          // Need to diff the size before and after to get the exact allocated space.
          const newLayerSize = Math.max(
            Math.min(sz + equalSpaceToAllocate, pane.max()),
            pane.min()
          );
          const allocated = newLayerSize - sz;
          leftToAllocate -= allocated;
          pane.setSz(newLayerSize);
        } else if (!unshrinkable.includes(pane)) {
          // Need to diff the size before and after to get the exact allocated space.
          const newLayerSize = Math.max(
            Math.min(sz + equalSpaceToAllocate, pane.max()),
            pane.min()
          );
          const allocated = newLayerSize - sz;
          leftToAllocate -= allocated;
          pane.setSz(newLayerSize);
        }
      }

    return leftToAllocate;
  }

  /**
	 * Checks that <Splitpanes> is composed of <Layer>, and verify that the panes are still in the right order,
		and if not update the internal order.
	 */
  function verifyAndUpdateLayersOrder() {
    if (!container) {
      return;
    }
    const { children } = container;
    let currentLayerIndex = 0;
    let needReorder = false;

    for (let i = 0; i < children.length; i++) {
      const child = children.item(i) as Element;
      const isLayer = child.classList.contains("splitlayers__layer");
      const isSplitter = child.classList.contains("splitlayers__splitter");

      // Node is not a Layer or a splitter: remove it.
      if (!isLayer && !isSplitter) {
        child.parentNode?.removeChild(child); // el.remove() doesn't work on IE11.
        console.warn(
          "Splitpanes: Only <Layer> elements are allowed at the root of <Splitpanes>. One of your DOM nodes was removed."
        );
        return;
      } else if (isLayer) {
        if (!needReorder && layers[currentLayerIndex].element !== child) {
          needReorder = true;
        }

        currentLayerIndex++;
      }
    }

    if (needReorder) {
      const newLayers = [];

      for (let i = 0; i < children.length; i++) {
        const child = children.item(i);
        const isLayer = child?.classList.contains("splitlayers__layer");
        if (isLayer) {
          const pane = layers.find((pane) => pane.element === child);
          if (pane) {
            pane.index = newLayers.length;
            newLayers.push(pane);
          } else {
            console.warn(
              "Splitpanes: Internal error - found a <Layer> elements which isn't tracked."
            );
          }
        }
      }

      layers = newLayers;
      $veryFirstLayerKey = layers.length > 0 ? layers[0].key : undefined;
    }
  }
</script>

<div
  {id}
  bind:this={container}
  class={`splitlayers ${theme || ""} ${clazz || ""}`}
  class:splitlayers--horizontal={horizontal}
  class:splitlayers--vertical={!horizontal}
  class:splitlayers--dragging={isMouseDown || isDragging}
  class:splitlayers--freeze={!isAfterInitialTimeoutZero}
  {style}
>
  <GatheringProvider>
    <slot />
  </GatheringProvider>
</div>

<style global lang="scss">
  @use "../../themes/default.scss";

  div.splitlayers--horizontal.splitlayers--dragging {
    cursor: row-resize;
  }
  div.splitlayers--vertical.splitlayers--dragging {
    cursor: col-resize;
  }

  .splitlayers {
    display: flex;
    width: 100%;
    height: 100%;
    &--vertical {
      flex-direction: row;
    }
    &--horizontal {
      flex-direction: column;
    }
    &--dragging * {
      user-select: none;
    }
    &__pane {
      width: 100%;
      height: 100%;
      overflow: hidden;
      .splitlayers--vertical & {
        transition: width 0.2s ease-out;
      }
      .splitlayers--horizontal & {
        transition: height 0.2s ease-out;
      }

      /** Add also a direct child selector, for dealing with specifity of nested splitlayers transition.
			    This issue was happening in the examples on nested splitlayers, vertical inside horizontal.
			    I think it's better to keep also the previous CSS selector for (potential) old browser compatibility.
			  */
      .splitlayers--vertical > & {
        transition: width 0.2s ease-out;
      }
      .splitlayers--horizontal > & {
        transition: height 0.2s ease-out;
      }

      .splitlayers--dragging & {
        transition: none;
        pointer-events: none;
      }

      .splitlayers--freeze & {
        transition: none;
      }
    }
    // Disable default zoom behavior on touch device when double tapping splitter.
    &__splitter {
      touch-action: none;
    }
    &--vertical > .splitlayers__splitter {
      min-width: 1px;
    }
    &--horizontal > .splitlayers__splitter {
      min-height: 1px;
    }
  }
</style>
