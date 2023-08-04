<template>
  <MainPane />
</template>

<script>
import HelloWorld from "./components/HelloWorld.vue";
import MainPane from "./components/MainPane.vue";

export default {
  name: "App",
  components: {
    MainPane,
    HelloWorld,
  },
};
</script>

<style>
.title {
  min-height: 30px;
  text-align: center;
}

.demo-tabs > .el-tabs__content {
  padding: 32px;
  color: #6b778c;
  font-size: 32px;
  font-weight: 600;
}
</style>
