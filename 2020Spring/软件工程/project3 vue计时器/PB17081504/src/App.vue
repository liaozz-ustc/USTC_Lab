<template>
  <div id="app">
      <tool-bar @sendTime="receiveTime" :enterKey="enterKey" :spaceKey="spaceKey"></tool-bar>
      <timer-container :time="time" ></timer-container>
  </div>
</template>

<script>
import ToolBar from './components/ToolBar'
import TimerContainer from './components/TimerContainer'
export default {
  name: 'App',
  components: {TimerContainer, ToolBar},
  data () {
    return {
      time: '00:00:00',
      enterKey: true,
      spaceKey: true
    }
  },
  methods: {
    receiveTime (time) {
      this.time = time
    }
  },
  mounted: function () {
    let that = this
    let keyboard = true
    document.onkeydown = function (event) {
      if (keyboard) {
        keyboard = false
        if (event.keyCode === 13) {
          that.enterKey = !that.enterKey
        } else if (event.keyCode === 32) {
          that.spaceKey = !that.spaceKey
        }
      }
    }
    document.onkeyup = function () {
      keyboard = true
    }
  }
}
</script>

<style>
#app {
    height: 510px;
    width: 1220px;
    margin: 0;
    padding: 0;
}
</style>
