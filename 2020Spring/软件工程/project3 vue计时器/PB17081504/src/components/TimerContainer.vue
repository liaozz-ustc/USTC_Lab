<template>
<div id = "timerContainer">
    <div id="mainTimer">
        <span id="time">{{time}}</span>
    </div>
</div>
</template>

<script>
export default {
  name: 'TimerContainer',
  props: ['time']
}

</script>

<style scoped>
    #timerContainer {
        position: relative;
        width: 1220px;
        height: 440px;
        background-color: #F2F4F7;
    }
    #mainTimer {
        position: absolute;
        width: 1140px;
        height: 440px;
        left: 40px;
        top: 0;
    }
    #time {
        position: absolute;
        left: 91px;
        top: 127px;
        width: 960px;
        height: 200px;
        opacity: 100%;
        color: #333333;
        font-size: 200px;
        line-height: 200px;
        font-family: PTMono-Bold, "PT Mono", monospace;
        font-weight: 700;
    }
</style>
