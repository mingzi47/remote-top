<template>
<table class="layout">
    <tr>
      <td colspan="2">
      <div><CpuPane/></div>
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
import { ref, inject, provide, onUnmounted } from "vue";
import CpuPane from "./CpuPane.vue";
import ProcPane from "./ProcPane.vue";
import NetPane from "./NetPane.vue";
import MemPane from "./MemPane.vue";
import { Collect } from "../../wailsjs/go/main/Collect";

export default {
  name: "MonitorPane",
  components: {
    CpuPane,
    ProcPane,
    NetPane,
    MemPane
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

    const eTabs = inject("Main_editableTabs")
    const eTabsValue = inject("Main_editableTabsValue")
    const ip = () => {
        for (let tab of eTabs.value) {
          if (tab.name == eTabsValue.value) {
            return tab.title;
          }
        }
        return "NewTab"
    };

    const monitorInfo = ref({});

    const mainTimer = setInterval(() => {
      if (ip().trim() == "New Tab") {
        return;
      }
      Collect(ip().trim(), 50051).then(result => {
        monitorInfo.value = result
      }).catch(error => {
        alert(error);
      })
    },2000)

    provide("MonitorInfo", monitorInfo);

    onUnmounted(() =>  {
      clearInterval(mainTimer);
    })

    return {
      percentage,
      customColorMethod,
      monitorInfo,
    }
  }
}
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
