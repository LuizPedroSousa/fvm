<script lang="ts">
  import * as Command from "$components/ui/command";
  import { onMount } from "svelte";

  let open = false;

  onMount(() => {
    function handleKeydown(e: KeyboardEvent) {
      if (e.key === "." && (e.metaKey || e.ctrlKey)) {
        e.preventDefault();
        open = !open;
      }
    }

    document.addEventListener("keydown", handleKeydown);
    return () => {
      document.removeEventListener("keydown", handleKeydown);
    };
  });

  let commands: any[] = [
    {
      name: "Entities",
      commands: [
        { name: "Add entity", shortcut: "ea" },
        { name: "Search", shortcut: "es" },
        { name: "Duplicate entity", shortcut: "ed" },
        { name: "Delete entity", shortcut: "ex" },
        { name: "Rename entity", shortcut: "er" },
        { name: "Move entity", shortcut: "em" },
        { name: "Toggle entity visibility", shortcut: "ev" },
        { name: "Focus on entity", shortcut: "ef" },
      ],
    },
    {
      name: "Components",
      commands: [
        { name: "Add component", shortcut: "ca" },
        { name: "Search", shortcut: "cs" },
        { name: "Remove component", shortcut: "cx" },
        { name: "Enable/Disable component", shortcut: "ce" },
        { name: "Edit component properties", shortcut: "cp" },
        { name: "Copy component", shortcut: "cc" },
        { name: "Paste component", shortcut: "cv" },
      ],
    },
    {
      name: "Project",
      commands: [
        { name: "New project", shortcut: "pn" },
        { name: "Open project", shortcut: "po" },
        { name: "Save project", shortcut: "ps" },
        { name: "Close project", shortcut: "pc" },
      ],
    },
    {
      name: "Scene",
      commands: [
        { name: "New scene", shortcut: "sn" },
        { name: "Open scene", shortcut: "so" },
        { name: "Save scene", shortcut: "ss" },
        { name: "Close scene", shortcut: "sc" },
        { name: "Play scene", shortcut: "sp" },
        { name: "Stop scene", shortcut: "sx" },
        { name: "Pause scene", shortcut: "sz" },
      ],
    },
    {
      name: "Editing",
      commands: [
        { name: "Undo", shortcut: "zu" },
        { name: "Redo", shortcut: "zr" },
        { name: "Cut", shortcut: "xc" },
        { name: "Copy", shortcut: "xv" },
        { name: "Paste", shortcut: "xp" },
        { name: "Delete", shortcut: "xd" },
      ],
    },
  ];
</script>

<Command.Dialog bind:open>
  <Command.Input placeholder="Type a command or search..." />
  <Command.List>
    <Command.Empty>No results found.</Command.Empty>
    {#each commands as command}
      <Command.Group heading={command.name}>
        {#each command?.commands as command}
          <Command.Item>
            <span>{command.name}</span>

            {#if command?.shortcut}
              <Command.Shortcut>⌘{command.shortcut}</Command.Shortcut>
            {/if}
          </Command.Item>
        {/each}
      </Command.Group>
      <Command.Separator />
    {/each}
  </Command.List>
</Command.Dialog>

<slot />
