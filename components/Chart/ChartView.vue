<script setup lang="ts">
import { ref } from 'vue';
import ChartContainer from '../Chart/ChartContainer.vue';
import Toolbar from '../Chart/Toolbar.vue';

const data = ref([
  { time: '2023-01-01', value: 100 },
  { time: '2023-01-02', value: 105 },
  { time: '2023-01-03', value: 102 },
]);

const chartType = ref('line');

function changeType(newType: string) {
  chartType.value = newType;
}

function changeData(newData: any[]) {
  data.value = newData;
}

const seriesOptions = ref({
  color: '#26a69a',
  upColor: '#26a69a',
  downColor: '#ef5350',
  borderVisible: false,
  wickUpColor: '#26a69a',
  wickDownColor: '#ef5350',
  wickVisible: true,
});

const chartRef = ref();

function fitChart() {
  chartRef.value?.fitContent();
}
</script>

<template>
  <div>
    <Toolbar
        @change-type="changeType"
        @change-data="changeData"
        @fit-content="fitChart"
    />
    <div class="chart-container">
      <ChartContainer
          ref="chartRef"
          :type="chartType"
          :data="data"
          :autosize="true"
          :series-options="seriesOptions"
      />
    </div>
  </div>
</template>

<style scoped>
.chart-container {
  height: 500px;
}
</style>