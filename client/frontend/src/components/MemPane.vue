<template>
  <el-card class="box-card">
    <template #header>
      <div class="card-header">
        <span>MEM</span>
      </div>
    </template>
    <div>
      <el-descriptions :column="2" border>
        <el-descriptions-item
          label="Mem Total"
          label-align="right"
          align="center"
          label-class-name="my-label"
          class-name="my-content"
          width="150px"
        >
          {{ memTotal }}
        </el-descriptions-item>
        <el-descriptions-item label="Free" label-align="right" align="center">
          {{ memFree }}
        </el-descriptions-item>
        <el-descriptions-item
          label="Swap Total"
          label-align="right"
          align="center"
        >
          {{ swapTotal }}
        </el-descriptions-item>
        <el-descriptions-item label="Free" label-align="right" align="center">
          {{ swapFree }}
        </el-descriptions-item>
      </el-descriptions>
    </div>
  </el-card>
</template>

<script>
import { inject, ref, onUnmounted } from "vue";
import { GetMem } from "../../wailsjs/go/main/Collect";

export default {
  name: "ProcPane",
  setup() {
    const disPlay = (mem) => {
      if (mem == undefined) return 0;
      const val = ["K", "M", "G"];
      let i = 0;
      for (; i < 3; ++i) {
        if (mem >= 1024) {
          mem /= 1024;
        } else {
          break;
        }
      }
      if (i >= 3) i = 2;
      return mem.toFixed(1) + val[i];
    };
    const memTotal = ref("null");
    const memFree = ref("null");
    const swapTotal = ref("null");
    const swapFree = ref("null");

    const tabTitle = inject("Monitor_tabTitle")
    const memTimer = setInterval(() => {
      GetMem(tabTitle.value, 50051).then((result) => {
        memTotal.value = disPlay(result.MemTotal);
        memFree.value = disPlay(result.MemFree);
        swapTotal.value = disPlay(result.SwapTotal);
        swapFree.value = disPlay(result.SwapFree);
        console.log(
          "debug mem : ",
          memTotal.value,
          memFree.value,
          swapTotal.value,
          swapFree.value
        );
      }).catch((error) => {
        alert(error)
      })
    }, 1500);

    onUnmounted(() => {
      clearInterval(memTimer);
    });

    return {
      memTotal,
      memFree,
      swapTotal,
      swapFree,
      tabTitle,
    };
  },
};
</script>

<style scoped>
.my-label {
  background: var(--el-color-success-light-9);
}
.my-content {
  background: var(--el-color-danger-light-9);
}

.card-header {
  justify-content: space-between;
  align-items: center;
}

.box-card {
  display: flex;
  /*height: 120px;
  width: 600px;*/
  width: 42vw;
  height: 15vh;
}
</style>
