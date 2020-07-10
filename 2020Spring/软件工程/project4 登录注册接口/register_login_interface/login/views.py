import json
from django.shortcuts import render
from django.shortcuts import redirect
from . import models

# Create your views here.


def login(request):
    if request.session.get('isLogin', None):
        status = 200
        message = "登录成功"
        return render(
            request, 'login/login.html', 
            {"status": json.dumps(status), "message": json.dumps(message)}
        )

    if request.method == "POST":
        username = request.POST.get('username')
        password = request.POST.get('password')
        message = "登录失败"
        status = 404
        if username.strip() and password:
            
            try:
                user = models.User.objects.get( username = username )
            except:
                return render(
                    request, 'login/login.html', 
                    {"status": json.dumps(status), "message": json.dumps(message)}
                )
            
            if user.password != password:
                return render(
                    request, 'login/login.html', 
                    {"status": json.dumps(status), "message": json.dumps(message)}
                )
            else:
                request.session['isLogin'] = True
                request.session['username'] = user.username
                request.session['userid'] = user.id
                status = 200
                message = "登录成功"
                return render(
                    request, 'login/login.html', 
                    {"status": json.dumps(status), "message": json.dumps(message)}
                )
    return render(request, 'login/login.html')


def index(request):
    if not request.session.get('isLogin', None):
        status = 404
        message = "未登录"
        if request.method == 'POST':
            return render(
                request, 'login/index.html',
                {"status": json.dumps(status), "message": json.dumps(message)}
            )

    if request.session.get('isLogin', None):
        request.session.set_expiry(300)
        if request.method == 'POST':
            date = request.POST.get('date')
            if date:
                dateList = date.split("-")
                if len(dateList) == 3:
                    if (dateList[0].isdigit() and len(dateList[0]) == 4 and
                        dateList[1].isdigit() and len(dateList[1]) == 2 and
                        dateList[2].isdigit() and len(dateList[2]) == 2):
                        if(dateList[1] == "01" and dateList[2] == "01"):
                            status = 200
                            message = "元旦"
                            return render(
                                request, 'login/index.html',
                                {"status": json.dumps(status), "message": json.dumps(message)}
                            )
                        elif (dateList[1] == "12" and dateList[2] == "25"):
                            status = 200
                            message = "圣诞节"
                            return render(
                                request, 'login/index.html',
                                {"status": json.dumps(status), "message": json.dumps(message)}
                            )                            
                        elif (dateList[1] == "10" and dateList[2] == "01"):
                            status = 200
                            message = "国庆节"
                            return render(
                                request, 'login/index.html',
                                {"status": json.dumps(status), "message": json.dumps(message)}
                            )

            return render(
                request, 'login/index.html',
                {}
            )
    
    return render(request, 'login/index.html')


def signup(request):
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        status = 404
        message = "注册失败"
        if username.strip() and password:
            user = models.User.objects.filter(username = username)
            if user:
                return render(
                    request, 'login/signup.html',
                    {"status": json.dumps(status), "message": json.dumps(message)}
                )
            newUser = models.User()
            newUser.username = username
            newUser.password = password        
            newUser.save()
            status = 200
            message = "注册成功"
            return render(
                request, 'login/signup.html',
                {"status": json.dumps(status), "message": json.dumps(message)}
            )

    return render(request, 'login/signup.html')


def logout(request):
    request.session.flush()
    return redirect("/index/")