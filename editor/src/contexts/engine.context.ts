import type { Writable } from "svelte/store";

export interface SocketEvent {
  topic: string
  topic_size: number
  content: Uint8Array;
}

export interface Component {
  id: string;
  name: string;
  owner_id: string;
  active: boolean
}

export interface Entity {
  id: string;
  name: string;
  components: Component[]
}


export interface Log {
  level: "info" | "warning" | "error" | "debug"
  timestamp: string
  message: string
  line: string
  caller: string
  file: string
}

interface Scene {
  entities: Record<string, Entity>
}

export type PaneId = "scene-hierachy" | "viewport" | "properties" | "console" | "content-browser"

export interface EngineContext {
  socket: Writable<WebSocket>
  socket_event: Writable<SocketEvent>;
  scene: Writable<Scene>
  logs: Writable<Log[]>
  activePaneId: Writable<string>
  selected_entity_id: Writable<string>;
  setActivePaneId: (id: PaneId) => void;
  send_event: (payload: any) => void;
  set_selected_entity: (id: string) => void;
}
