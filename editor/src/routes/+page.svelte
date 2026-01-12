<script lang="ts">
  import LayerHandle from "$components/layers/layer-handle.svelte";
  import Layer from "$components/layers/layer.svelte";
  import SplitLayer from "../components/layers/split-layer.svelte";
  import Engine from "../contexts/engine.svelte";
  import Command from "../contexts/command.svelte";
  import ConsoleLayer from "../views/console-layer/console-layer.svelte";
  import PropertiesLayer from "../views/properties-layer/properties-layer.svelte";
  import SceneHierarchyLayer from "../views/scene-hierarchy-layer.svelte";
  import ViewportLayer from "../views/viewport-layer.svelte";
  import Menubar from "../templates/menubar.svelte";
  import * as Tabs from "$components/ui/tabs";
  import ContentBrowser from "../views/content-browser/content-browser.svelte";
  import LayerContent from "$components/layers/layer-content.svelte";
</script>

<Engine>
  <Command>
    <main class="wrapper">
      <Menubar />
      <SplitLayer direction="vertical">
        <Layer>
          <SplitLayer>
            <SceneHierarchyLayer layer={{ minSize: 10, defaultSize: 10 }} />
            <LayerHandle />
            <ViewportLayer />
            <LayerHandle />
            <PropertiesLayer layer={{ minSize: 10, defaultSize: 10 }} />
          </SplitLayer>
        </Layer>
        <LayerHandle />

        <Layer
          default_pane="content-browser"
          panes={[
            {
              id: "content-browser",
              name: "Content Browser",
              icon: "flat-color-icons:folder",
            },
            {
              id: "console",
              name: "Console",
              icon: "material-symbols:terminal",
            },
          ]}
          {...{ defaultSize: 20 }}
        >
          <LayerContent id="console">
            <ConsoleLayer layer={{ minSize: 10, defaultSize: 10 }} />
          </LayerContent>
          <ContentBrowser />
        </Layer>
      </SplitLayer>
    </main>
  </Command>
</Engine>

<style>
  .wrapper {
    height: 100vh;
    display: flex;
    flex-direction: column;
    width: 100vw;
  }
</style>
