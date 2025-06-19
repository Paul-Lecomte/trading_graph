<template>
  <div ref="chartContainer" class="chart-container">
    <div id="legend" class="legend"></div>
  </div>
</template>

<script setup lang="ts">
import {
  createChart,
  LineSeries,
  AreaSeries,
  BarSeries,
  CandlestickSeries,
  HistogramSeries,
  BaselineSeries,
  type IChartApi,
  type ISeriesApi,
} from 'lightweight-charts';
import {
  ref,
  onMounted,
  onUnmounted,
  watch,
  defineExpose,
  nextTick,
} from 'vue';

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
let series: ISeriesApi<any> | null = null;
const legend = ref<HTMLElement | null>(null);

const defaultChartOptions = {
  layout: {
    background: { color: '#181A20' },
    textColor: '#C3BCDB',
    fontFamily: 'Inter, Arial, sans-serif',
  },
  grid: {
    vertLines: { color: '#222' },
    horzLines: { color: '#222' },
  },
  timeScale: {
    borderColor: '#333',
    timeVisible: true,
    secondsVisible: false,
  },
  rightPriceScale: {
    borderColor: '#333',
    textColor: '#C3BCDB',
  },
  crosshair: {
    mode: 0,
    vertLine: { color: '#758696', width: 1 as const, style: 2 as const, visible: true, labelVisible: false },
    horzLine: { color: '#758696', width: 1 as const, style: 2 as const, visible: true, labelVisible: false },
  },
};

let loadedBars = 5000;

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

function sliceBars(data: any[]) {
  const len = data.length;
  const start = Math.max(0, len - loadedBars);
  return data.slice(start, len);
}

function onCrosshairMove(param: any) {
  if (
      !param ||
      param.time === undefined ||
      param.point.x < 0 ||
      param.point.y < 0 ||
      !legend.value
  ) return;

  const bar = props.data.find((d: any) => d.time === param.time);
  if (!bar) return;

  legend.value.innerText = `O: ${bar.open ?? '-'} | H: ${bar.high ?? '-'} | L: ${bar.low ?? '-'} | C: ${bar.close ?? '-'}`;
}

function onVisibleLogicalRangeChanged(range: any) {
  if (!series || !props.data) return;

  const barsInfo = series.barsInLogicalRange(range);
  if (barsInfo && barsInfo.barsBefore < 50) {
    loadedBars += 5000;
    const sliced = sliceBars(props.data);
    series.setData(sliced);
  }
}

onMounted(async () => {
  await nextTick();
  legend.value = document.getElementById('legend');

  chart = createChart(
      chartContainer.value!,
      { ...defaultChartOptions, ...(props.chartOptions ?? {}) }
  );
  series = chart.addSeries(getSeriesConstructor(props.type), props.seriesOptions ?? {});
  series.setData(sliceBars(props.data));

  chart.timeScale().fitContent();
  if (props.timeScaleOptions) chart.timeScale().applyOptions(props.timeScaleOptions);
  if (props.priceScaleOptions) chart.priceScale().applyOptions(props.priceScaleOptions);

  chart.subscribeCrosshairMove(onCrosshairMove);
  chart.timeScale().subscribeVisibleLogicalRangeChange(onVisibleLogicalRangeChanged);

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
    series = chart.addSeries(getSeriesConstructor(newType), props.seriesOptions ?? {});
    series.setData(sliceBars(props.data));
  }
});

watch(() => props.data, (newData) => {
  if (series) {
    series.setData(sliceBars(newData));
  }
});

watch(() => props.chartOptions, (opts) => chart?.applyOptions({ ...defaultChartOptions, ...(opts ?? {}) }));
watch(() => props.seriesOptions, (opts) => series?.applyOptions(opts ?? {}));
watch(() => props.timeScaleOptions, (opts) => chart?.timeScale().applyOptions(opts ?? {}));
watch(() => props.priceScaleOptions, (opts) => chart?.priceScale().applyOptions(opts ?? {}));

function fitContent() {
  chart?.timeScale().fitContent();
}

defineExpose({ fitContent });
</script>

<style scoped>
.chart-container {
  height: 400px;
  width: 100%;
  position: relative;
  background: #181A20;
  border-radius: 8px;
  box-shadow: 0 2px 8px #000a;
  padding: 8px;
}

.legend {
  position: absolute;
  top: 4px;
  left: 4px;
  background: rgba(255, 255, 255, 0.9);
  padding: 4px 8px;
  font-size: 12px;
  border-radius: 4px;
  z-index: 10;
  pointer-events: none;
  font-family: monospace;
}
</style>