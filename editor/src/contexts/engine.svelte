<script lang="ts">
  import { onMount, setContext } from "svelte";
  import { writable } from "svelte/store";
  import type { EngineContext, Entity, Log, PaneId } from "./engine.context";
  import type { SocketEvent } from "./engine.context";

  let ws: WebSocket;
  let socket = writable<WebSocket>();
  let socket_event = writable<SocketEvent>();
  let scene = writable({ entities: {} });
  let activePaneId = writable<PaneId>();

  let selected_entity_id = writable<string>();
  let logs = writable<Log[]>([]);

  onMount(() => {
    ws = new WebSocket("ws://localhost:9001");

    ws.binaryType = "arraybuffer";
    ws.onopen = () => {
      console.log("WebSocket connection established");
      socket.set(ws);
    };

    ws.onerror = (error: any) => {
      console.error("WebSocket error:", error);
    };

    ws.onclose = () => {
      console.log("WebSocket connection closed");
    };

    ws.onmessage = (event: any) => {
      if (event.data instanceof ArrayBuffer) {
        const topic_size = new DataView(event.data).getUint32(0, false);

        const decoder = new TextDecoder();

        const topic = decoder.decode(new Uint8Array(event.data, 4, topic_size));

        const payload = {
          topic,
          content: new Uint8Array(event.data, 4 + topic_size),
          topic_size: topic_size,
        };

        socket_event.set(payload);

        const parser_by_topic: Record<string, () => void> = {
          scene: () => {
            const _scene = JSON.parse(decoder.decode(payload.content));

            const entities: Record<string, Entity> = {};

            if (_scene?.entities?.length > 0) {
              for (let entity of _scene.entities) {
                entities[entity.id] = entity;
              }
            }

            scene.set({
              ..._scene,
              entities,
            });
          },
          logs: () => {
            const _logs = JSON.parse(decoder.decode(payload.content));

            logs.set(_logs);
          },
        };

        const parser = parser_by_topic[topic];

        if (!parser) {
          return;
        }

        parser();
      } else {
        console.warn("Received non-binary data");
      }
    };

    return () => {
      ws.close();
    };
  });

  const send_event = (payload: any) => {
    if (ws?.readyState !== WebSocket.OPEN) {
      return;
    }

    ws.send(JSON.stringify(payload));
  };

  const set_selected_entity = (id: string) => {
    selected_entity_id.set(id);
  };

  const setActivePaneId = (id: PaneId) => {
    activePaneId.set(id);
  };

  setContext("engine", {
    socket,
    selected_entity_id,
    socket_event,
    send_event,
    set_selected_entity,
    scene,
    activePaneId,
    setActivePaneId,
    logs,
  } as EngineContext);
</script>

<slot />
