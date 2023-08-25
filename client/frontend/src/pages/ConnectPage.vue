<script>
import { ref, inject, provide } from "vue";
import MonitorPage from "./MonitorPage.vue";
import { CreateConn } from "../../wailsjs/go/main/Collect";

export default {
  name: "ConnectPage",
  setup() {
    const ip_text = ref(" ");

    const editableTabs = inject("Main_editableTabs");
    const editableTabsValue = inject("Main_editableTabsValue");
    function onConnect() {
      console.log("ip : " + ip_text.value);
      CreateConn(ip_text.value.trim(), 50051)
        .then((result) => {
          editableTabs.value.forEach((tab) => {
            if (tab.name == editableTabsValue.value) {
              tab.title = ip_text.value.trim();
              tab.content = MonitorPage;
            }
          });
        })
        .catch((error) => {
          alert(error);
        });
    }
    return {
      ip_text,
      onConnect,
      MonitorPage,
    };
  },
};
</script>

<template>
  <table>
    <tr>
      <th colspan="3" class="center1"></th>
    </tr>
    <tr>
      <th class="center2"></th>
      <th>
        <div>
          <el-card class="box-card">
            <el-form-item label="ip">
              <el-input v-model="ip_text" />
            </el-form-item>
            <el-form-item>
              <el-button @click="onConnect">Connect</el-button>
              <el-button>Cancel</el-button>
            </el-form-item>
          </el-card>
        </div>
      </th>
      <th class="center2"></th>
    </tr>
    <tr>
      <th colspan="3" class="center1"></th>
    </tr>
  </table>
</template>

<style scoped>
.box-card {
  width: 25vw;
  height: 15vh;
}
.center1 {
  width: 97vw;
  height: 30vh;
}

.center2 {
  width: 35vw;
}
</style>
