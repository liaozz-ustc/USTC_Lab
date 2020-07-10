var hour = 0;
var minute = 0;
var second = 0;
var milliSecond = 0;
var setId;
var setHour;
var setMinute;
var setSecond;
var upOrDown;
var state = "counter";
var countup=document.getElementById('countup');
var countdown=document.getElementById('countdown');
var clear=document.getElementById('clear');
var restart=document.getElementById('restart');
var resume=document.getElementById('resume');
var pause=document.getElementById('pause');

function startCountUp() {
    milliSecond += 10;
    if (milliSecond === 1000) {
        milliSecond = 0;
        second++;
        if (second === 60) {
            second = 0;
            minute++;
            if (minute === 60) {
                minute = 0;
                hour++;
                if (hour === 100) {
                    hour = 0;
                }
            }
        }
        if (hour === setHour && minute === setMinute && second === setSecond) {
            endUpTime();
        }
        document.getElementById('time').innerHTML = timeToString(hour, minute, second);
    }
}

function timeToString(hour, minute, second) {
    var string = "";
    if (hour < 10) {
        string = "0" + hour + ":";
    } else {
        string = hour + ":";
    }
    if (minute < 10) {
        string = string + "0" + minute + ":";
    } else {
        string = string + minute + ":";
    }
    if (second < 10) {
        string = string + "0" + second;
    } else {
        string = string + second;
    }
    return string;
}

countup.onclick = function () {
    countUpTime();
};

function countUpTime() {
    upOrDown = 1;
    getTime();
    if (setHour === 0 && setMinute === 0 && setSecond === 0)
        return 1;
    display("counting");
    state = "counting";
    document.getElementById('hint').innerHTML = "正在正计时 " + timeToString(setHour, setMinute, setSecond);
    document.getElementById('clear').innerText = '清空正计时';
    setId = setInterval(startCountUp, 10);
}
function checkTime() {
    if (setHour >= 100) {
        setHour = 99;
    } else if (setHour < 0) {
        setHour = 0;
    }
    if (setMinute >= 60) {
        setMinute = 59;
    } else if (setMinute < 0) {
        setMinute = 0;
    }
    if (setSecond >= 60) {
        setSecond = 59;
    } else if (setSecond < 0) {
        setSecond = 0;
    }
}

pause.onclick = function () {
    pauseTime();
};

function pauseTime() {
    clearInterval(setId);
    state = "pause";
    if (upOrDown === 1)
        document.getElementById('hint').innerHTML = "暂停正计时 " + timeToString(setHour, setMinute, setSecond);
    else
        document.getElementById('hint').innerHTML = "暂停倒计时 " + timeToString(setHour, setMinute, setSecond);
    document.getElementById('pause').style.display = "none";
    document.getElementById('resume').style.display = "inline-block";
}

function endUpTime() {
    clearInterval(setId);
    state = "counted";
    document.getElementById('hint').innerHTML = "正计时 " + timeToString(setHour, setMinute, setSecond) + " 已结束";
    document.getElementById('pause').style.display = "none";
}

function endDownTime() {
    clearInterval(setId);
    state = "counted";
    document.getElementById('hint').innerHTML = "倒计时 " + timeToString(setHour, setMinute, setSecond) + " 已结束";
    document.getElementById('pause').style.display = "none";
}

resume.onclick = function () {
    resumeTime();
};

function resumeTime() {
    if (upOrDown === 1) {
        setId = setInterval(startCountUp, 10);
        state = "counting";
        document.getElementById('hint').innerHTML = "正在正计时 " + timeToString(setHour, setMinute, setSecond);
    } else {
        setId = setInterval(startCountDown, 10);
        state = "counting";
        document.getElementById('hint').innerHTML = "正在倒计时 " + timeToString(setHour, setMinute, setSecond);
    }
    document.getElementById('resume').style.display = "none";
    document.getElementById('pause').style.display = "inline-block";
}

function startCountDown() {
    milliSecond -= 10;
    if (milliSecond === 0) {
        if (hour === 0 && minute === 0 && second === 0) {
            endDownTime();
        } else if (second === 0) {
            if (minute === 0) {
                hour--;
                minute = 59;
                second = 59;
            } else {
                minute--;
                second = 59;
            }
        } else {
            second--;
        }
        milliSecond = 1000;
        document.getElementById('time').innerHTML = timeToString(hour, minute, second);
    }
}

clear.onclick = function () {
    clearInterval(setId);
    hour = 0;
    minute = 0;
    second = 0;
    milliSecond = 0;
    state = "counter";
    display("counter");
    var tag = document.getElementsByClassName("inputFont");
    for (var i = 0; i < tag.length; i++) {
        tag[i].value = 0;
    }
    document.getElementById("time").innerHTML = "00:00:00";
};

restart.onclick = function () {
    clearInterval(setId);
    display("counting");
    if (upOrDown === 1) {
        hour = 0;
        minute = 0;
        second = 0;
        milliSecond = 0;
        state = "counting";
        document.getElementById('hint').innerHTML = "正在正计时 " + timeToString(setHour, setMinute, setSecond);
        document.getElementById("time").innerHTML = "00:00:00";
        setId = setInterval(startCountUp, 10);
    } else {
        countDownTime();
    }

};

function countDownTime(){
    state = "counting";
    document.getElementById('hint').innerHTML = "正在倒计时 " + timeToString(setHour, setMinute, setSecond);
    document.getElementById('clear').innerText = '清空倒计时';
    outTime();
    document.getElementById('time').innerHTML = timeToString(hour, minute, second);
    setId = setInterval(startCountDown, 10);
}

countdown.onclick = function () {
    upOrDown = 0;
    getTime();
    if (setHour === 0 && setMinute === 0 && setSecond === 0)
        return 1;
    display("counting");
    countDownTime()
};

function display(className) {
    var tag = document.getElementById('inputField').children;
    for (var i = 0; i < tag.length; i++) {
        tag[i].style.display = "none";
    }
    tag = document.getElementsByClassName(className);
    for (i = 0; i < tag.length; i++) {
        tag[i].style.display = "inline-block";
    }
}

function getTime() {
    setHour = Math.round(Number(document.getElementById('hour').value));
    setMinute = Math.round(Number(document.getElementById('minute').value));
    setSecond = Math.round(Number(document.getElementById('second').value));
    checkTime();
}

function outTime() {
    hour = setHour;
    minute = setMinute;
    second = setSecond;
    milliSecond = 1000;
    if (second === 0) {
        if (minute === 0) {
            hour--;
            minute = 59;
            second = 59;
        } else {
            minute--;
            second = 59;
        }
    } else {
        second--;
    }
}

document.onkeypress = function (event) {
    var EVENT = event || window.event;
    if (EVENT && state === "counter" && EVENT.keyCode === 13) {
        countUpTime();
    }
    if (EVENT && state === "counting" && EVENT.keyCode === 32) {
        pauseTime();
    } else if (EVENT && state === "pause" && EVENT.keyCode === 32) {
        resumeTime();
    }
};
