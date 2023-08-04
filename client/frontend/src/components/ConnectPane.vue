<script>
import { ref, inject } from "vue"
import MonitorPane from "./MonitorPane.vue"
import { Collect } from "../../wailsjs/go/main/Collect"

export default {
  name: 'ConnectPane',
  setup() {
    const ip_text = ref(' ')

    const editableTabs = inject("Main_editableTabs")
    const editableTabsValue = inject("Main_editableTabsValue")
   function onConnect(){
    console.log("ip : " + ip_text.value);
    Collect(ip_text.value.trim(), 50051).then(result => {
      editableTabs.value.forEach((tab) => {
        if (tab.name == editableTabsValue.value) {
          tab.title = ip_text;
          tab.content = MonitorPane;
        }
      });
    }).catch(error => {
      alert(error)
    });
  };
    return {
      ip_text,
      onConnect,
      MonitorPane,

    }
  }
}
</script>

<template>
  <el-row>
<el-col :span="8"></el-col>
<el-col :span="8">
    <el-card class="box-card">
        <el-form-item label="ip">
          <el-input v-model="ip_text" />
        </el-form-item>
        <el-form-item>
          <el-button @click="onConnect">Connect</el-button>
          <el-button>Cancel</el-button>
        </el-form-item>
    </el-card>
</el-col>
<el-col :span="8"></el-col>
</el-row>
</template>

<style scoped>
.el-row {
  margin-top: 10%;
}
.text {
  font-size: 14px;
}


.box-card {
  width: 480px;
}

.div-centers {
  position:absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
}
</style>
