<script setup lang="ts">
import { ref } from 'vue';
import ChartContainer from '@/components/Chart/ChartContainer.vue';
import Toolbar from '@/components/Chart/Toolbar.vue';

// Génération données line
function generateLineData() {
  const data = [];
  const base = Math.floor(Date.now() / 1000);
  for (let i = 0; i < 100; i++) {
    data.push({ time: base + i * 60, value: Math.random() * 300 });
  }
  return data;
}

// Génération données candlestick bugged for some reason meeh
function generateCandleData() {
  const data = [];
  const base = Math.floor(Date.now() / 1000);
  for (let i = 0; i < 100; i++) {
    const open = Math.random() * 100;
    const close = open + (Math.random() - 0.5) * 10;
    const high = Math.max(open, close) + Math.random() * 5;
    const low = Math.min(open, close) - Math.random() * 5;
    data.push({
      time: base + i * 60,
      open: Number(open.toFixed(2)),
      high: Number(high.toFixed(2)),
      low: Number(low.toFixed(2)),
      close: Number(close.toFixed(2)),
    });
  }
  return data;
}

const chartType = ref('line');
const chartData = ref(generateLineData());

// Options série (line ou candlestick)
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

function updateType(type: string) {
  chartType.value = type;

  if (type === 'candlestick') {
    chartData.value = generateCandleData();
    seriesOptions.value = {
      upColor: '#4caf50',
      downColor: '#f44336',
      borderVisible: true,
      borderColor: '#000000',
      wickVisible: true,
      wickColor: '#000000',
    };
  } else {
    chartData.value = generateLineData();
    seriesOptions.value = {
      color: '#2962FF',
    };
  }
}
function changeColors() {
  if (chartType.value === 'candlestick') {
    const upColor = '#' + Math.floor(Math.random() * 16777215).toString(16);
    const downColor = '#' + Math.floor(Math.random() * 16777215).toString(16);
    const borderColor = '#' + Math.floor(Math.random() * 16777215).toString(16);
    const wickColor = '#' + Math.floor(Math.random() * 16777215).toString(16);

    seriesOptions.value = {
      ...seriesOptions.value,
      upColor,
      downColor,
      borderColor,
      wickColor,
      borderVisible: true,
      wickVisible: true,
    };
  } else {
    seriesOptions.value = {
      color: '#' + Math.floor(Math.random() * 16777215).toString(16),
    };
  }
}

function changeData() {
  if (chartType.value === 'candlestick') {
    chartData.value = generateCandleData();
  } else {
    chartData.value = generateLineData();
  }
}

function fitChart() {
  chartRef.value?.fitContent();
}
</script>

<template>
  <Toolbar
      @change-type="updateType"
      @change-data="changeData"
      @change-colors="changeColors"
      @fit-content="fitChart"
  />

  <ChartContainer
      ref="chartRef"
      :type="chartType"
      :data="chartData"
      :series-options="seriesOptions"
  />
</template>
