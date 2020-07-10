<template>
    <button v-bind:id="buttonType" @click="beClicked" :style="'background-color: ' + color + ';' + 'left: ' + position + ';'">{{message}}</button>
</template>

<script>
export default {
  name: 'ButtonTime',
  props: ['buttonType', 'state', 'upOrDown', 'color', 'position'],
  computed: {
    message: function () {
      if (this.buttonType === 'countup') {
        return '开始正计时'
      } else if (this.buttonType === 'countdown') {
        return '开始倒计时'
      } else if (this.buttonType === 'clear') {
        if (this.upOrDown === 'up') {
          return '清空正计时'
        } else {
          return '清空倒计时'
        }
      } else if (this.buttonType === 'restart') {
        return '重新再计时'
      } else if (this.buttonType === 'resume') {
        return '恢复计时器'
      } else if (this.buttonType === 'pause') {
        return '暂停计时器'
      }
    }
  },
  methods: {
    beClicked: function () {
      this.$emit('click')
    }
  }
}
</script>

<style scoped>
    button {
        width: 98px;
        height: 40px;
        top: 15px;
        opacity: 100%;
        border: none;
        box-shadow: 0 2px 4px 0 rgba(0, 0, 0, 0.10);
        border-radius: 5px;
        text-align: center;
        line-height: 22px;
        letter-spacing: 0;
        font-family: PingFangSC-Regular, "PingFang SC", sans-serif;
        color: #FFFFFF;
        font-size: 16px;
        position: absolute;
    }
</style>
