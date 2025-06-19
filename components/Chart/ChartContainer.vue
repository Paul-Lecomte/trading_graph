<script setup lang="ts">
import { ref, onMounted, onUnmounted, watch, defineProps, defineExpose } from 'vue';
import {
  createChart,
  LineSeries,
  AreaSeries,
  BarSeries,
  CandlestickSeries,
  HistogramSeries,
  BaselineSeries,
} from 'lightweight-charts';

import type { IChartApi, ISeriesApi } from 'lightweight-charts';

const props = defineProps({
  type: { type: String, default: 'line' },
  data: { type: Array, required: true },
  autosize: { type: Boolean, default: true },
  chartOptions: Object,
  seriesOptions: Object,
  timeScaleOptions: Object,
  priceScaleOptions: Object,
});

const chartContainer = ref<HTMLElement>();
let chart: IChartApi | null = null;
let series: ISeriesApi | null = null;

function getSeriesConstructor(type: string) {
  return {
    line: LineSeries,
    area: AreaSeries,
    bar: BarSeries,
    candlestick: CandlestickSeries,
    histogram: HistogramSeries,
    baseline: BaselineSeries,
  }[type.toLowerCase()] || LineSeries;
}

function resize() {
  if (chart && chartContainer.value) {
    const { width, height } = chartContainer.value.getBoundingClientRect();
    chart.resize(width, height);
  }
}

onMounted(() => {
  chart = createChart(chartContainer.value!, props.chartOptions);
  series = chart.addSeries(getSeriesConstructor(props.type), props.seriesOptions);
  series.setData(props.data);

  if (props.timeScaleOptions) chart.timeScale().applyOptions(props.timeScaleOptions);
  if (props.priceScaleOptions) chart.priceScale().applyOptions(props.priceScaleOptions);

  chart.timeScale().fitContent();

  if (props.autosize) {
    window.addEventListener('resize', resize);
  }
});

onUnmounted(() => {
  if (chart) {
    chart.remove();
    chart = null;
  }
  series = null;
  window.removeEventListener('resize', resize);
});

watch(() => props.type, (newType) => {
  if (chart && series) {
    chart.removeSeries(series);
    series = chart.addSeries(getSeriesConstructor(newType), props.seriesOptions);
    series.setData(props.data);
  }
});

watch(() => props.data, (newData) => {
  series?.setData(newData);
});

watch(() => props.chartOptions, (opts) => chart?.applyOptions(opts));
watch(() => props.seriesOptions, (opts) => series?.applyOptions(opts));
watch(() => props.timeScaleOptions, (opts) => chart?.timeScale().applyOptions(opts));
watch(() => props.priceScaleOptions, (opts) => chart?.priceScale().applyOptions(opts));

function fitContent() {
  chart?.timeScale().fitContent();
}

defineExpose({ fitContent });
</script>

<template>
  <div ref="chartContainer" style="height: 400px; width: 100%;"></div>
</template>
