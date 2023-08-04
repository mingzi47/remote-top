<template>

<el-card class="box-card">
  <template #header>
    <div class="card-header">
      <span>PROC</span>
    </div>
  </template>
  <el-table
    :data="tableData"
    height="30.5vh"
    :default-sort="{ prop: 'Pid', order: 'ascending' }"
    style="width: 100%"
  >
    <el-table-column prop="Pid" label="Pid" sortable />
    <el-table-column prop="Name" label="Name" />
    <el-table-column prop="User" label="User" />
    <el-table-column prop="State" label="State" />
    <el-table-column prop="Thread" label="Thread" />
    <el-table-column prop="Cpu" label="Cpu" :formatter="fCpu" sortable />
    <el-table-column prop="Mem" label="Mem" :formatter="fMem" sortable />
  </el-table>
</el-card>
</template>

<script>

import { inject, ref, onUnmounted } from 'vue';

export default {
  name : "ProcPane",
  setup() {
    const tableData = ref(null);
    const fCpu = (row, col) => {
      return row.Cpu.toFixed(1) + "%";
    }
    const fMem = (row, col) => {
      const val = ['K', 'M', 'G'];
      let i = 0;
      for (; i < 3; ++i) {
        if (row.Mem >= 1024) {
          row.Mem /= 1024;
        } else {
          break;
        }
      }
      if (i >= 3) i = 2;
      return row.Mem.toFixed(1) + val[i];
    }
    const MonitorInfo = inject("MonitorInfo");

    const procTimer = setInterval(() => {
      tableData.value = MonitorInfo.value.Procs;
    },2000)

    onUnmounted(() =>  {
      clearInterval(procTimer);
    })

    return {
      tableData,
      fCpu,
      fMem,
    }
  }
}

</script>

<style scoped>
.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.box-card {
  /*width: 830px;*/
  width: 52.5vw;
}
</style>

