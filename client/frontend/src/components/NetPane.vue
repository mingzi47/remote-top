<template>
  <el-card class="box-card">
    <template #header>
      <div class="card-header">
        <span>NET</span>
      </div>
    </template>
    <el-table
      :data="tableData"
      height="15vh"
      :default-sort="{ prop: 'download_s', order: 'descending' }"
      style="width: 100%"
    >
      <el-table-column prop="Name" label="Name" />
      <el-table-column
        prop="UploadS"
        label="Upload"
        :formatter="fUs"
        sortable
      />
      <el-table-column
        prop="DownloadS"
        label="Download"
        :formatter="fDs"
        sortable
      />
      <el-table-column
        prop="Upload"
        label="Upload
            Total"
        :formatter="fU"
        sortable
      />
      <el-table-column
        prop="Download"
        label="Download
            Total"
        :formatter="fD"
        sortable
      />
    </el-table>
  </el-card>
</template>

<script>
import { inject, ref, onUnmounted } from "vue";
import { GetNets } from "../../wailsjs/go/main/Collect";

export default {
  name: "NetPane",
  setup() {
    const tableData = ref(null);

    const fDs = (row, col) => {
      const val = ["K/s", "M/s", "G/s"];
      let i = 0;
      for (; i < 3; ++i) {
        if (row.DownloadS >= 1024) {
          row.DownloadS /= 1024;
        } else {
          break;
        }
      }
      if (i >= 3) i = 2;
      return row.DownloadS.toFixed(1) + val[i];
    };
    const fUs = (row, col) => {
      const val = ["K/s", "M/s", "G/s"];
      let i = 0;
      for (; i < 3; ++i) {
        if (row.UploadS >= 1024) {
          row.UploadS /= 1024;
        } else {
          break;
        }
      }
      if (i >= 3) i = 2;
      return row.UploadS.toFixed(1) + val[i];
    };
    const fD = (row, col) => {
      const val = ["K", "M", "G"];
      let i = 0;
      for (; i < 3; ++i) {
        if (row.Download >= 1024) {
          row.Download /= 1024;
        } else {
          break;
        }
      }
      if (i >= 3) i = 2;
      return row.Download.toFixed(1) + val[i];
    };
    const fU = (row, col) => {
      const val = ["K", "M", "G"];
      let i = 0;
      for (; i < 3; ++i) {
        if (row.Upload >= 1024) {
          row.Upload /= 1024;
        } else {
          break;
        }
      }
      if (i >= 3) i = 2;
      return row.Upload.toFixed(1) + val[i];
    };

    const tabTitle = inject("Monitor_tabTitle")
    const netTimer = setInterval(() => {
      GetNets(tabTitle.value, 50051).then((result) => {
        tableData.value = result;
      }).catch((error) => {
        console.log(error)
      })
    }, 2000);

    onUnmounted(() => {
      clearInterval(netTimer);
    });
    return {
      tableData,
      fUs,
      fU,
      fDs,
      fD,
      tabTitle,
    };
  },
};
</script>

<style scoped>
.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.box-card {
  /*width: 600px;*/
  width: 42vw;
}
</style>
