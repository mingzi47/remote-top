<template>
  <el-tabs
    v-model="editableTabsValue"
    type="card"
    editable
    class="demo-tabs"
    @edit="handleTabsEdit"
  >
    <el-tab-pane
      v-for="item in editableTabs"
      :key="item.name"
      :label="item.title"
      :name="item.name"
    >
      <component :is="item.content" @connect="onConnect"></component>
    </el-tab-pane>
  </el-tabs>
</template>

<script>
import ConnectPane from "./ConnectPane.vue";
import MonitorPane from "./MonitorPane.vue";
import { ref, provide } from "vue";
export default {
  name: "MainPane",
  components: {
    ConnectPane,
    MonitorPane,
  },
  setup() {
    let tabIndex = 1;
    const editableTabsValue = ref("1");
    const editableTabs = ref([
      {
        title: "New Tab",
        name: "1",
        content: ConnectPane,
      },
    ]);

    const handleTabsEdit = (targetName, action) => {
      if (action === "add") {
        const newTabName = `${++tabIndex}`;
        editableTabs.value.push({
          title: "New Tab",
          name: newTabName,
          content: ConnectPane,
        });
        editableTabsValue.value = newTabName;
      } else if (action === "remove") {
        const tabs = editableTabs.value;
        let activeName = editableTabsValue.value;
        if (activeName === targetName) {
          tabs.forEach((tab, index) => {
            if (tab.name === targetName) {
              const nextTab = tabs[index + 1] || tabs[index - 1];
              if (nextTab) {
                activeName = nextTab.name;
              }
            }
          });
        }

        editableTabsValue.value = activeName;
        editableTabs.value = tabs.filter((tab) => tab.name !== targetName);
      }
    };

    provide("Main_editableTabsValue", editableTabsValue);
    provide("Main_editableTabs", editableTabs);

    return {
      tabIndex,
      editableTabsValue,
      editableTabs,
      handleTabsEdit,
    };
  },
};
</script>

<style scoped>
.demo-tabs > .el-tabs__content {
  padding: 2vh;
  color: #6b778c;
  font-size: 32px;
  font-weight: 600;
}
</style>
