<template>
    <div class="inputValue" >
        <div id="inputDivTime">
            <label>
            <input type="number" :id="timeUnit" class="inputFont" value=0 v-model="number" @change="setNumber">
            {{timeToChinese}}
            </label>
        </div>
    </div>
</template>

<script>
export default {
  name: 'InputTime',
  props: ['timeUnit'],
  data () {
    return {
      number: 0
    }
  },
  computed: {
    timeToChinese: function () {
      if (this.timeUnit === 'hour') { return '时' } else if (this.timeUnit === 'minute') { return '分' } else if (this.timeUnit === 'second') { return '秒' }
    }
  },
  methods: {
    setNumber: function () {
      this.$emit('transferNumber', this.number)
    }
  }
}
</script>

<style scoped>
    .inputValue {
        width: 70px;
        height: 40px;
        top: 15px;
        opacity: 100%;
        background-color: #FFFFFF;
        box-shadow: 0 2px 4px 0 rgba(0, 0, 0, 0.10);
        border-radius: 5px;
    }
    input {
        font-family: PingFangSC-Regular, "PingFang SC", sans-serif;
        width: 31px;
        height: 22px;
        opacity: 100%;
        color: #AFAFAF;
        font-size: 16px;
        border: none;
        line-height: 22px;
        letter-spacing: 0;
    }
    label {
        height: 22px;
        top: 9px;
        opacity: 100%;
        font-family: PingFangSC-Regular, "PingFang SC", sans-serif;
        color: #222222;
        font-size: 16px;
        line-height: 22px;
        letter-spacing: 0;
    }
    #inputDivTime {
        position: absolute;
        left: 9px;
        top: 9px;
    }
</style>
