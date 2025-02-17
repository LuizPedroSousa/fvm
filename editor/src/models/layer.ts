export interface Layer {
  key: any;
  element: HTMLElement;
  index: number;
  min: () => number;
  max: () => number;
  snap: () => number;
  sz: () => number;
  setSz: (number: number) => void;
  setSplitterActive: (isActive: boolean) => void;
  givenSize: number | null;
  isReady: boolean;
}

export interface LayerSizingEvent {
  min: number;
  max: number;
  size: number;
  snap: number;
}
