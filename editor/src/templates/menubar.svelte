<script lang="ts">
  import * as Menubar from "../components/ui/menubar";
  import MenuItem from "./menu-item.svelte";

  const menus: any[] = [
    {
      name: "File",
      items: [
        { name: "New Project", shortcut: "N" },
        { name: "Open Project", shortcut: "O" },
        { name: "Save Project", shortcut: "S" },
        { name: "Save Project As" },
        { type: "separator" },
        { name: "New Scene", shortcut: "Ctrl+N" },
        { name: "Open Scene", shortcut: "Ctrl+O" },
        { name: "Save Scene", shortcut: "Ctrl+S" },
        { name: "Save Scene As" },
        { type: "separator" },
        { name: "Import Assets" },
        { name: "Export" },
        { type: "separator" },
        { name: "Exit" },
      ],
    },
    {
      name: "Edit",
      items: [
        { name: "Undo", shortcut: "Z" },
        { name: "Redo", shortcut: "Y" },
        { type: "separator" },
        { name: "Cut", shortcut: "X" },
        { name: "Copy", shortcut: "C" },
        { name: "Paste", shortcut: "V" },
        { name: "Delete", shortcut: "Del" },
      ],
    },
    {
      name: "Assets",
      items: [
        {
          name: "Create",
          items: [
            { name: "Folder" },
            { name: "Script" },
            { name: "Material" },
            { name: "Shader" },
          ],
        },
        { name: "Import Asset" },
        { name: "Reimport All" },
        { type: "separator" },
        { name: "Show in Explorer" },
      ],
    },
    {
      name: "GameObject",
      items: [
        { name: "Create Empty" },
        { name: "Create Empty Child" },
        { type: "separator" },
        {
          name: "3D Object",
          items: [{ name: "Cube" }, { name: "Sphere" }, { name: "Capsule" }],
        },
        {
          name: "Light",
          items: [
            { name: "Directional Light" },
            { name: "Point Light" },
            { name: "Spot Light" },
          ],
        },
        { name: "Camera" },
      ],
    },
    {
      name: "View",
      items: [
        { name: "Scene" },
        { name: "Game" },
        { name: "Inspector" },
        { name: "Hierarchy" },
        { name: "Project" },
        { name: "Console" },
      ],
    },
    {
      name: "Help",
      items: [
        { name: "Documentation" },
        { name: "Tutorials" },
        { name: "About" },
      ],
    },
  ];
</script>

<Menubar.Root>
  {#each menus as menu, index}
    <Menubar.Menu>
      <Menubar.Trigger>{menu.name}</Menubar.Trigger>
      <Menubar.Content>
        {#each menu?.items as item}
          {#if item.type === "separator"}
            <Menubar.Separator />
          {:else}
            <MenuItem {item} />
          {/if}
        {/each}
      </Menubar.Content>
    </Menubar.Menu>
  {/each}
</Menubar.Root>
