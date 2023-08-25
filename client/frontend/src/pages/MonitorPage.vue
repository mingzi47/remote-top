<template>
  <table class="layout">
    <tr>
      <td colspan="2">
        <div><CpuPane /></div>
      </td>
    </tr>
    <tr>
      <td>
        <div><MemPane /></div>
      </td>
      <td rowspan="2">
        <div><ProcPane /></div>
      </td>
    </tr>
    <tr>
      <td>
        <div><NetPane /></div>
      </td>
    </tr>
  </table>
</template>

<script>
import { ref, inject, provide, onUnmounted, onBeforeMount } from "vue";
import CpuPane from "../components/CpuPane.vue";
import ProcPane from "../components/ProcPane.vue";
import NetPane from "../components/NetPane.vue";
import MemPane from "../components/MemPane.vue";
import { Delete } from "../../wailsjs/go/main/Collect";

export default {
  name: "MonitorPage",
  components: {
    CpuPane,
    ProcPane,
    NetPane,
    MemPane,
  },
  setup() {
    const percentage = ref(8);

    const customColorMethod = (percentage) => {
      if (percentage < 50) {
        return "#67c23a";
      }
      if (percentage < 80) {
        return "#e6a23c";
      }
      return "#FF0000";
    };


    const tabTitle = ref("New Tab")

    provide("Monitor_tabTitle", tabTitle);

    const setTabTitle = () => {
      const eTabs = inject("Main_editableTabs");
      const eTabsValue = inject("Main_editableTabsValue");
      const ip = () => {
        for (let tab of eTabs.value) {
          if (tab.name == eTabsValue.value) {
            return tab.title.trim();
          }
        }
        return "NewTab";
      };
      tabTitle.value = ip();
    }

    onBeforeMount(() => {
      setTabTitle()
      console.log("tabTitle" , tabTitle.value);
    })

    onUnmounted(() => {
      Delete(tabTitle, 50051)
    });

    return {
      percentage,
      customColorMethod,
      tabTitle,
    };
  },
};
</script>

<style>
.el-row {
  margin-bottom: 20px;
}
.el-row:last-child {
  margin-bottom: 0;
}
.el-col {
  border-radius: 4px;
}

.grid-content {
  border-radius: 4px;
  min-height: 36px;
}

.layout {
}
</style>
