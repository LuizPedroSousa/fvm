import type { Readable } from "svelte/store";
import type { Layer } from "../models/layer";

export interface ClientCallback {
  onSplitterDown: (_event: TouchEvent | MouseEvent) => void;
  onSplitterClick: (_event: MouseEvent) => void;
  onSplitterDblClick: (_event: MouseEvent) => void;
  onLayerClick: (_event: MouseEvent) => void;
  reportGivenSizeChange: (_newGivenSize: number) => void;
}

export type LayerInitFunction = (_key: any) => {
  undefinedLayerInitSize: number;
};

export interface GatheringContext {
  veryFirstLayerKey: Readable<any>;
  isHorizontal: Readable<boolean>;
  showFirstSplitter: Readable<boolean>;
  onLayerInit: LayerInitFunction;
  clientOnly?: {
    onLayerAdd: (layer: Layer) => ClientCallback;
    onLayerRemove: (key: any) => Promise<void>;
  };
}
