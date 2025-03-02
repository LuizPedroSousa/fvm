import type { Writable } from "svelte/store";

export interface SocketEvent {
  topic: string
  topic_size: number
  content: Uint8Array;
}

export interface Entity {
  id: string;
  name: string;
}

interface Scene {
  entities: Record<string, Entity>
}

export interface EngineContext {
  socket?: Writable<WebSocket>
  socket_event?: Writable<SocketEvent>;
  scene: Writable<Scene>
  selected_entity_id?: Writable<string>;
  send_event: (payload: any) => void;
  set_selected_entity: (id: string) => void;
}
