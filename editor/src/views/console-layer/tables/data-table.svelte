<script lang="ts">
  import { getContext } from "svelte";
  import {
    createRender,
    createTable,
    Render,
    Subscribe,
  } from "svelte-headless-table";
  import { readable } from "svelte/store";
  import type { EngineContext } from "../../../contexts/engine.context";
  import * as Table from "$components/ui/table";
  import DataTableActions from "./data-table-actions.svelte";
  import {
    addResizedColumns,
    addSortBy,
    addTableFilter,
  } from "svelte-headless-table/plugins";
  import { Button } from "$components/ui/button";
  import Icon from "@iconify/svelte";
  import { Input } from "$components/ui/input";
  import { Badge } from "$components/ui/badge";

  const { logs } = getContext<EngineContext>("engine");

  const table = createTable(readable($logs || []), {
    sort: addSortBy(),
    resize: addResizedColumns({}),
    filter: addTableFilter({
      fn: ({ filterValue, value }) =>
        value.toLowerCase().includes(filterValue.toLowerCase()),
    }),
  });

  const columns = table.createColumns([
    table.column({
      accessor: "timestamp",
      header: "Timestamp",
      plugins: {
        sort: {
          disable: false,
        },
        filter: {
          exclude: true,
        },
      },
    }),
    table.column({
      accessor: "file",
      header: "File",
      plugins: {
        sort: {
          disable: true,
        },
      },
    }),
    table.column({
      accessor: "line",
      header: "Line",
      plugins: {
        sort: {
          disable: true,
        },
      },
    }),
    table.column({
      accessor: "caller",
      header: "Caller",
      plugins: {
        sort: {
          disable: true,
        },
      },
    }),
    table.column({
      accessor: "level",
      header: "Level",
      plugins: {
        sort: {
          disable: true,
        },
      },
    }),
    table.column({
      accessor: "message",
      header: "Message",
      plugins: {
        sort: {
          disable: true,
        },
      },
    }),
    table.column({
      accessor: ({ id }) => id,
      header: "",
      cell: ({ value }) => {
        return createRender(DataTableActions, { id: value });
      },
    }),
  ]);

  const { headerRows, pageRows, tableAttrs, tableBodyAttrs, pluginStates } =
    table.createViewModel(columns);

  const { filterValue } = pluginStates.filter;

  const { columnWidths } = pluginStates.resize;
</script>

<div>
  <div class="flex items-center py-4">
    <Input
      class="max-w-2xl"
      placeholder="Search log"
      type="text"
      bind:value={$filterValue}
    />
  </div>
  <div class="rounded-md border">
    <Table.Root {...$tableAttrs}>
      <Table.Header>
        {#each $headerRows as headerRow}
          <Subscribe rowAttrs={headerRow.attrs()}>
            <Table.Row>
              {#each headerRow.cells as cell (cell.id)}
                <Subscribe
                  attrs={cell.attrs()}
                  let:attrs
                  props={cell.props()}
                  let:props
                >
                  <Table.Head {...attrs}>
                    <Button variant="ghost" on:click={props.sort.toggle}>
                      <Render of={cell.render()} />
                      <Icon
                        icon="iconamoon:arrow-down-2"
                        class={"ml-2 h-4 w-4"}
                      />
                    </Button>
                  </Table.Head>
                </Subscribe>
              {/each}
            </Table.Row>
          </Subscribe>
        {/each}
      </Table.Header>

      <Table.Body {...$tableBodyAttrs}>
        {#each $pageRows as row (row.id)}
          <Subscribe rowAttrs={row.attrs()} let:rowAttrs>
            <Table.Row {...rowAttrs}>
              {#each row.cells as cell (cell.id)}
                <Subscribe attrs={cell.attrs()} let:attrs>
                  <Table.Cell {...attrs}>
                    {#if cell.id === "level"}
                      <Badge>
                        <Render of={cell.render()} />
                      </Badge>
                    {:else}
                      <Render of={cell.render()} />
                    {/if}
                  </Table.Cell>
                </Subscribe>
              {/each}
            </Table.Row>
          </Subscribe>
        {/each}
      </Table.Body>
    </Table.Root>
  </div>
</div>
