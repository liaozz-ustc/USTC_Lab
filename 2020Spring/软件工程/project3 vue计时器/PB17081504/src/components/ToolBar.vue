<template>
    <div id="toolBar">
        <div id="inputField">
            <input-time id="inputHours" :time-unit="'hour'" v-if="state==='counter'" @transferNumber="getHour"></input-time>
            <input-time id="inputMinutes" :time-unit="'minute'" v-if="state==='counter'" @transferNumber="getMinute"></input-time>
            <input-time id="inputSeconds" :time-unit="'second'" v-if="state==='counter'" @transferNumber="getSecond"></input-time>
            <span id="hint" v-if="state!=='counter'">{{message}}</span>
            <button-time :button-type="'countup'" :state="state" v-if="state==='counter'" :up-or-down="upOrDown" @click="countUpTime" :color="'#2188E9'" :position="'270px'"></button-time>
            <button-time :button-type="'countdown'" :state="state" v-if="state==='counter'" :up-or-down="upOrDown" @click="countDownTime" :color="'#2188E9'" :position="'388px'"></button-time>
            <button-time :button-type="'clear'" :state="state" v-if="state!=='counter'" :up-or-down="upOrDown" @click="clearCounting" :color="'#DD2E1D'" :position="'924px'"></button-time>
            <button-time :button-type="'restart'" :state="state" v-if="state!=='counter'" :up-or-down="upOrDown" @click="restartCounting" :color="'#FFB020'" :position="'1042px'"></button-time>
            <button-time :button-type="'resume'" :state="state" v-if="state==='paused'" :up-or-down="upOrDown" @click="resumeTime" :color="'#2188E9'" :position="'187px'"></button-time>
            <button-time :button-type="'pause'" :state="state" v-if="state==='counting'" :up-or-down="upOrDown" @click="pauseTime" :color="'#2188E9'" :position="'187px'"></button-time>
        </div>
    </div>
</template>

<script>
import InputTime from './InputTime'
import ButtonTime from './ButtonTime'
export default {
  name: 'ToolBar',
  components: {ButtonTime, InputTime},
  props: ['enterKey', 'spaceKey'],
  data () {
    return {
      setHour: 0,
      setMinute: 0,
      setSecond: 0,
      setTime: '00:00:00',
      setId: null,
      hour: 0,
      minute: 0,
      second: 0,
      milliSecond: 0,
      time: '00:00:00',
      upOrDown: 'up',
      state: 'counter'
    }
  },
  methods: {
    getHour (msg) {
      this.setHour = msg
    },
    getMinute (msg) {
      this.setMinute = msg
    },
    getSecond (msg) {
      this.setSecond = msg
    },
    timeToString (hour, minute, second) {
      let time = '00:00:00'
      if (hour < 10) {
        time = '0' + hour + ':'
      } else {
        time = hour + ':'
      }
      if (minute < 10) {
        time = time + '0' + minute + ':'
      } else {
        time = time + minute + ':'
      }
      if (second < 10) {
        time = time + '0' + second
      } else {
        time = time + second
      }
      return time
    },
    checkTime () {
      if (this.setHour >= 100) {
        this.setHour = 99
      } else if (this.setHour < 0) {
        this.setHour = 0
      }
      if (this.setMinute >= 60) {
        this.setMinute = 59
      } else if (this.setMinute < 0) {
        this.setMinute = 0
      }
      if (this.setSecond >= 60) {
        this.setSecond = 59
      } else if (this.setSecond < 0) {
        this.setSecond = 0
      }
      this.setSecond = Math.round(Number(this.setSecond))
      this.setMinute = Math.round(Number(this.setMinute))
      this.setHour = Math.round(Number(this.setHour))
    },
    countUpTime () {
      this.upOrDown = 'up'
      this.hour = this.minute = this.second = this.milliSecond = 0
      this.checkTime()
      if (this.setHour === 0 && this.setMinute === 0 && this.setSecond === 0) {
        return 1
      }
      this.setTime = this.timeToString(this.setHour, this.setMinute, this.setSecond)
      this.state = 'counting'
      this.setId = setInterval(this.startCountUp, 10)
    },
    startCountUp () {
      this.milliSecond += 10
      if (this.milliSecond === 1000) {
        this.milliSecond = 0
        this.second++
        if (this.second === 60) {
          this.second = 0
          this.minute++
          if (this.minute === 60) {
            this.minute = 0
            this.hour++
            if (this.hour === 100) {
              this.hour = 0
            }
          }
        }
        if (this.hour === this.setHour && this.minute === this.setMinute && this.second === this.setSecond) {
          this.endTime()
        }
      }
    },
    endTime () {
      clearInterval(this.setId)
      this.state = 'counted'
    },
    pauseTime () {
      clearInterval(this.setId)
      this.state = 'paused'
    },
    resumeTime () {
      if (this.upOrDown === 'up') {
        this.setId = setInterval(this.startCountUp, 10)
        this.state = 'counting'
      } else {
        this.setId = setInterval(this.startCountDown, 10)
        this.state = 'counting'
      }
    },
    startCountDown () {
      this.milliSecond -= 10
      if (this.milliSecond === 0) {
        if (this.hour === 0 && this.minute === 0 && this.second === 0) {
          this.endTime()
        } else if (this.second === 0) {
          if (this.minute === 0) {
            this.hour--
            this.minute = 59
            this.second = 59
          } else {
            this.minute--
            this.second = 59
          }
        } else {
          this.second--
        }
        this.milliSecond = 1000
      }
    },
    countDownTime () {
      this.upOrDown = 'down'
      this.checkTime()
      if (this.setHour === 0 && this.setMinute === 0 && this.setSecond === 0) {
        return 1
      }
      this.setTime = this.timeToString(this.setHour, this.setMinute, this.setSecond)
      this.state = 'counting'
      this.outTime()
      this.setId = setInterval(this.startCountDown, 10)
    },
    outTime () {
      this.hour = this.setHour
      this.minute = this.setMinute
      this.second = this.setSecond
      this.milliSecond = 1000
      if (this.second === 0) {
        if (this.minute === 0) {
          this.hour--
          this.minute = 59
          this.second = 59
        } else {
          this.minute--
          this.second = 59
        }
      } else {
        this.second--
      }
    },
    clearCounting () {
      clearInterval(this.setId)
      this.hour = this.minute = this.second = this.milliSecond = 0
      this.setHour = this.setMinute = this.setSecond = 0
      this.state = 'counter'
    },
    restartCounting () {
      clearInterval(this.setId)
      this.state = 'counter'
      if (this.upOrDown === 'up') {
        this.countUpTime()
      } else {
        this.countDownTime()
      }
    }
  },
  computed: {
    message: function () {
      if (this.state === 'counting') {
        if (this.upOrDown === 'up') {
          return '正在正计时 ' + this.setTime
        } else {
          return '正在倒计时 ' + this.setTime
        }
      } else if (this.state === 'paused') {
        if (this.upOrDown === 'up') {
          return '暂停正计时 ' + this.setTime
        } else {
          return '暂停倒计时 ' + this.setTime
        }
      } else if (this.state === 'counted') {
        if (this.upOrDown === 'up') {
          return '正计时 ' + this.setTime + ' 已结束'
        } else {
          return '倒计时 ' + this.setTime + ' 已结束'
        }
      }
    }
  },
  watch: {
    second: function () {
      this.$emit('sendTime', this.timeToString(this.hour, this.minute, this.second))
    },
    enterKey: function () {
      if (this.state === 'counter') {
        this.countUpTime()
      }
    },
    spaceKey: function () {
      if (this.state === 'counting') {
        this.pauseTime()
      } else if (this.state === 'paused') {
        this.resumeTime()
      }
    }
  }
}
</script>

<style scoped>
    #toolBar {
        width: 1220px;
        height: 70px;
        opacity: 100%;
        background-color: #97A5BC;
        box-shadow: 0 2px 4px 0 rgba(0, 0, 0, 0.10);
    }
    #inputField {
        position: absolute;
        left: 40px;
        top: 0;
        width: 1140px;
        height: 70px;
        opacity: 100%;
    }
    #inputHours {
        position: absolute;
        left: 0;
    }

    #inputMinutes {
        position: absolute;
        left: 90px;
    }

    #inputSeconds {
        position: absolute;
        left: 180px;
    }
    #hint {
        position: absolute;
        left: 0;
        top: 24px;
        width: 192px;
        height: 22px;
        opacity: 100%;
        font-family: PingFangSC-Regular, "PingFang SC", sans-serif;
        color: #FFFFFF;
        font-size: 16px;
        line-height: 22px;
        letter-spacing: 0;
    }
</style>
