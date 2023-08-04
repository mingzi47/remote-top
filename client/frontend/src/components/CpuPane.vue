<template>
  <el-card class="box-card">
    <template #header>
      <div class="card-header">
        <span>CPU</span>
        <span>{{ cpu_name }}</span>
      </div>
    </template>
    <div class="horizontal-container">
      <div class="main_pane">
        <v-chart :option="main_option" class="main_cpu" />
      </div>
      <div class="core_pane">
        <v-chart :option="core_option" class="core_cpu" />
      </div>
    </div>
  </el-card>
</template>

<script>
import { ref, computed, inject, onUnmounted } from "vue";

export default {
  setup() {
    const main_data = ref(0);
    const core_data = ref({
      name: ["null"],
      data: [
        {
          value: 0,
          itemStyle: {
            color: "#67c23a",
          },
        },
      ],
    });
    const cpu_name = ref("null");

    const setColor = (value) => {
      if (value <= 50) {
        return "#67c23a";
      } else if (value <= 75) {
        return "#e6a23c";
      }
      return "#FF0000";
    };

    const cpuInfo = inject("MonitorInfo");
    const cpuTimer = setInterval(() => {
      main_data.value = cpuInfo.value.Cpu.MainData.toFixed(1);
      const tmp = cpuInfo.value.Cpu.CoreData;
      cpu_name.value = cpuInfo.value.Cpu.Name;
      core_data.value.name = tmp.Name;
      core_data.value.data = tmp.map((value) => {
        return {
          value: value.Value,
          itemStyle: {
            color: setColor(value.Value),
          },
        };
      });
    }, 2000);

    const main_option = computed(() => {
      return {
        series: [
          {
            type: "gauge",
            axisLine: {
              lineStyle: {
                width: 15,
                color: [
                  [0.5, "#67c23a"],
                  [0.75, "#e6a23c"],
                  [1, "#FF0000"],
                ],
              },
            },
            pointer: {
              itemStyle: {
                color: "auto",
              },
            },
            axisTick: {
              distance: -30,
              length: 8,
              lineStyle: {
                color: "#fff",
                width: 2,
              },
            },
            splitLine: {
              distance: -30,
              length: 30,
              lineStyle: {
                color: "#fff",
                width: 4,
              },
            },
            axisLabel: {
              color: "inherit",
              distance: 20,
              fontSize: 15,
            },
            detail: {
              valueAnimation: true,
              formatter: "{value} %",
              color: "inherit",
              fontSize: 15,
            },
            data: [
              {
                value: main_data.value,
              },
            ],
          },
        ],
      };
    });

    const core_option = computed(() => {
      return {
        xAxis: {
          type: "category",
          data: core_data.name,
        },
        yAxis: {
          type: "value",
          max: 100,
          min: 0,
          interval: 20,
          axisLabel: {
            formatter: function (val) {
              return val + "%";
            },
          },
        },
        series: [
          {
            data: core_data.value.data,
            type: "bar",
          },
        ],
      };
    });

    onUnmounted(() => {
      clearInterval(cpuTimer);
    });
    return {
      main_option,
      main_data,
      core_option,
      core_data,
      cpu_name,
    };
  },
};
</script>

<style scoped>
.main_cpu {
  width: 30vw;
  height: 30vh;
}
/*.core_cpu {
  height: 320px;
  width: 1000px;
}*/
/*.main_cpu {
  height: 300px;
  width: 400px;
}*/
.core_pane {
  height: 30vh;
  width: 60vw;
}

.horizontal-container {
  display: flex;
  flex-direction: row; /* 横向排列子元素 */
}

.card-header {
  justify-content: space-between;
  align-items: center;
  display: flex;
}

.box-card {
  /* width: 1440px;
  height: 400px; */
  width: 95vw;
  height: 42vh;
}
</style>
