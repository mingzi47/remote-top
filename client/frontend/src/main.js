import { createApp } from "vue";
import ElementPlus from "element-plus";
import "element-plus/dist/index.css";
import ECharts from "vue-echarts";
import App from "./App.vue";
import "echarts";

createApp(App).use(ElementPlus).component("v-chart", ECharts).mount("#app");
