<script setup lang="ts">
const emit = defineEmits<{
  (e: 'change-type', type: string): void;
  (e: 'change-data'): void;
  (e: 'change-colors'): void;
  (e: 'fit-content'): void;
}>();

function changeToLine() {
  emit('change-type', 'line');
}
function changeToCandle() {
  emit('change-type', 'candlestick');
}
</script>

<template>
  <div class="toolbar">
    <button @click="changeToLine">Line</button>
    <button @click="changeToCandle">Candlestick</button>
    <button @click="$emit('change-data')">Changer les données</button>
    <button @click="$emit('change-colors')">Couleurs aléatoires</button>
    <button @click="$emit('fit-content')">Fit Content</button>
  </div>
</template>

<style scoped>
.toolbar {
  display: flex;
  gap: 10px;
  margin-bottom: 10px;
}
button {
  padding: 6px 12px;
  border: none;
  background-color: #2d4dcd;
  color: white;
  cursor: pointer;
  border-radius: 4px;
}
button:hover {
  background-color: #223aac;
}
</style>
