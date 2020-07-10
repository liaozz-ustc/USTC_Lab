import json
from django.forms.models import model_to_dict
from django.shortcuts import render
from django.shortcuts import redirect
from django.http import HttpResponse
from django.core.serializers.json import DjangoJSONEncoder
from django.db.models import Avg, Max, Min, Count, Sum
import datetime
import pytz

utc = pytz.UTC
import time
from . import models
from decimal import Decimal

chinese = ['', '第一季度', '第二季度', '第三季度', '第四季度']


# Create your views here.

# class DateEncoder(json.JSONEncoder):
#     def default(self, obj):
#         if isinstance(obj,datetime.datetime):
#             return obj.strftime("%Y-%m-%d %H:%M:%S")
#         else:
#             return json.JSONEncoder.default(self,obj)

class DateEncoder(DjangoJSONEncoder):  # class DateEncoder(json.JSONEncoder):
    """
     #解决时间字段json问题
    """

    def default(self, obj):
        if isinstance(obj, datetime.datetime):
            return obj.strftime("%Y-%m-%d %H:%M:%S")
        else:
            # return json.JSONEncoder.default(self, obj)  #  UUID 等类型字段时报错
            return DjangoJSONEncoder.default(self, obj)


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
                user = models.User.objects.get(username=username)
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
        request.session.set_expiry(0)
        if request.method == 'POST':
            date = request.POST.get('date')
            if date:
                dateList = date.split("-")
                if len(dateList) == 3:
                    if (dateList[0].isdigit() and len(dateList[0]) == 4 and
                            dateList[1].isdigit() and len(dateList[1]) == 2 and
                            dateList[2].isdigit() and len(dateList[2]) == 2):
                        if (dateList[1] == "01" and dateList[2] == "01"):
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
            user = models.User.objects.filter(username=username)
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


def addCustomer(request):
    if request.method == 'POST':
        customerID = request.POST.get('customerID')
        # 已存在的客户无法再增加
        customer = models.Customer.objects.filter(customerid=customerID)
        if customer:
            return render(request, 'addCustomer.html',
                          {"status": json.dumps(400), "message": json.dumps("该客户已存在")})
        customerName = request.POST.get('customerName')
        address = request.POST.get('address')
        phone = request.POST.get('phone')
        # 负责人员工需在数据库中
        staffID = request.POST.get('staffID')
        try:
            staffID = models.Staff.objects.get(staffid=staffID)
        except:
            return render(request, 'addCustomer.html',
                          {"status": json.dumps(400), "message": json.dumps("员工未找到，客户增加失败")})
        linkmanName = request.POST.get('linkmanName')
        linkmanPhone = request.POST.get('linkmanPhone')
        linkmanEmail = request.POST.get('linkmanEmail')
        linkmanRelationship = request.POST.get('linkmanRelationship')
        createTime = request.POST.get('createTime')
        newCustomer = models.Customer()
        newCustomer.customerid = customerID
        newCustomer.customername = customerName
        newCustomer.address = address
        newCustomer.phone = phone
        newCustomer.staffid = staffID
        newCustomer.linkmanname = linkmanName
        newCustomer.linkmanphone = linkmanPhone
        newCustomer.linkmanemail = linkmanEmail
        newCustomer.linkmanrelationship = linkmanRelationship
        # 前端传来日期，后端自动补上保存时时间
        time = datetime.datetime.now()
        newCustomer.createtime = createTime + ' ' + str(time.hour) + ':' + str(time.minute) + ':' + str(time.second)
        newCustomer.save()
        status = 200
        message = '客户增加成功'
        return render(request, 'addCustomer.html', {"status": json.dumps(status), "message": json.dumps(message)})
    return render(request, 'addCustomer.html')


def getTime(date):
    time = datetime.datetime.now()
    dateTime = date + ' ' + str(time.hour) + ':' + str(time.minute) + ':' + str(time.second)
    return dateTime


def deleteCustomer(request):
    # 记住删除的时候要将相应的bankCustomer删去,设置了级联删除不用自己写
    # 有账户的时候不能删除，因此当客户无账户能删除时在bankCustomer中无记录了
    if request.method == "POST":
        customerID = request.POST.get('customerID')
        customer = models.Customer.objects.get(customerid=customerID)
        try:
            customer.delete()
        except:
            return render(request, 'deleteCustomer.html',
                          {"code": json.dumps(400), "msg": json.dumps("存在关联账户或贷款，删除失败")})
        return render(request, 'deleteCustomer.html', {"code": json.dumps(200), "msg": json.dumps("客户删除成功")})
    return render(request, 'deleteCustomer.html')


def modifyCustomer(request):
    if request.method == 'POST':
        customerID = request.POST.get('customerID')
        customerName = request.POST.get('customerName')
        address = request.POST.get('address')
        phone = request.POST.get('phone')
        staffID = request.POST.get('staffID')
        linkmanName = request.POST.get('linkmanName')
        linkmanPhone = request.POST.get('linkmanPhone')
        linkmanEmail = request.POST.get('linkmanEmail')
        linkmanRelationship = request.POST.get('linkmanRelationship')
        createTime = request.POST.get('createTime')
        staffID = models.Staff.objects.filter(staffid=staffID)
        if not staffID:
            return render(request, 'modifyCustomer.html', {"code": json.dumps(400), "msg": json.dumps("员工不存在")})
        newCustomer = models.Customer.objects.get(customerid=customerID)
        newCustomer.customername = customerName
        newCustomer.address = address
        newCustomer.phone = phone
        newCustomer.staffid = staffID[0]
        newCustomer.linkmanname = linkmanName
        newCustomer.linkmanphone = linkmanPhone
        newCustomer.linkmanemail = linkmanEmail
        newCustomer.linkmanrelationship = linkmanRelationship
        # 前端传来日期，后端自动补上保存时时间
        time = datetime.datetime.now()
        newCustomer.createtime = createTime + ' ' + str(time.hour) + ':' + str(time.minute) + ':' + str(time.second)
        newCustomer.save()
        return render(request, 'modifyCustomer.html', {"code": json.dumps(200), "msg": json.dumps("客户信息修改成功")})
    return render(request, 'modifyCustomer.html')


def searchCustomer(request):
    return render(request, 'searchCustomer.html')


def searchCustomerCondition(request):
    code = "400"
    msg = ""
    if request.method == 'POST':
        searchType = request.POST.get('searchType')
        if searchType[-1] == '1':
            customers = models.Customer.objects.filter(customerid=request.POST.get('customerID'))
            if not customers:
                # 如果客户不存在
                if searchType[0] == 'S':
                    return render(request, 'searchCustomer.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyCustomer.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteCustomer.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
            jsonData = queryToList(customers)
        elif searchType[-1] == '2':
            customers = models.Customer.objects.filter(customername=request.POST.get('customerName'))
            if not customers:
                if searchType[0] == 'S':
                    return render(request, 'searchCustomer.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyCustomer.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteCustomer.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
            jsonData = queryToList(customers)
        elif searchType[-1] == '3':
            # 一个账号可能有多个客户
            accounts = models.Savecustomer.objects.filter(accountid=request.POST.get('accountID'))
            if not accounts:
                if searchType[0] == 'S':
                    return render(request, 'searchCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
            # 账户存在一定有客户
            lis = []
            for i in range(accounts.count()):
                lis.append(model_to_dict(accounts[i].customerid))
            # print(lis)
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        elif searchType[-1] == '4':
            # 一个账号可能有多个客户
            accounts = models.Chequecustomer.objects.filter(accountid=request.POST.get('accountID'))
            if not accounts:
                if searchType[0] == 'S':
                    return render(request, 'searchCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
            # 账户存在一定有客户
            lis = []
            for i in range(accounts.count()):
                lis.append(model_to_dict(accounts[i].customerid))
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        elif searchType[-1] == 5:
            # 一个账号可能有多个客户
            accounts = models.Customerloan.objects.filter(loanid=request.POST.get('accountID'))

            if not accounts:
                if searchType[0] == 'S':
                    return render(request, 'searchCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteCustomer.html', {"code": json.dumps(400), "msg": json.dumps("账户不存在")})
            # 账户存在一定有客户
            lis = []
            for i in range(accounts.count()):
                lis.append(model_to_dict(accounts[i].customerid))
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        else:
            bank = models.Banksubbranch.objects.filter(bankname=request.POST.get('bankName'))
            if not bank:
                if searchType[0] == 'S':
                    return render(request, 'searchCustomer.html', {"code": json.dumps(400), "msg": json.dumps("支行不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyCustomer.html', {"code": json.dumps(400), "msg": json.dumps("支行不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteCustomer.html', {"code": json.dumps(400), "msg": json.dumps("支行不存在")})
            lis = []
            for bankCustomer in models.Bankcustomer.objects.filter(bankname=bank[0].bankname):
                lis.append(model_to_dict(bankCustomer.customerid))
            for i in range(len(lis)):
                for j in range(i + 1, len(lis)):
                    if lis[i]['customerid'] == lis[j]['customerid']:
                        lis[i] = {}
                        break
            lis = [i for i in lis if i != {}]
            # print(lis)
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        if searchType[0] == 'S':
            return render(request, 'searchCustomer.html', jsonData)
        elif searchType[0] == 'M':
            return render(request, 'modifyCustomer.html', jsonData)
        elif searchType[0] == 'D':
            return render(request, 'deleteCustomer.html', jsonData)
    return render(request, 'searchCustomer.html', {"msg": msg})


def searchAccountCondition(request):
    code = "400"
    msg = ""
    if request.method == 'POST':
        searchType = request.POST.get('searchType')
        if searchType[-1] == '1':
            customers = models.Customer.objects.filter(customerid=request.POST.get('customerID'))
            if not customers:
                # 如果客户不存在
                if searchType[0] == 'S':
                    return render(request, 'searchAccount.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyAccount.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteAccount.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'E':
                    return render(request, 'depositWithdrawal.html',
                                  {"code": json.dumps(400), "msg": json.dumps("客户不存在")})

            savecustomer = customers[0].savecustomer_set.all()
            chequecustomer = customers[0].chequecustomer_set.all()
            # print(chequecustomer.count())
            lis = []
            for i in savecustomer:
                save = model_to_dict(i.accountid)
                save['accounttype'] = '储蓄账户'
                save['customerid'] = customers[0].customerid
                lis.append(save)
            for i in chequecustomer:
                cheque = model_to_dict(i.accountid)
                cheque['accounttype'] = '支票账户'
                cheque['customerid'] = customers[0].customerid
                lis.append(cheque)
            # print(lis)
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        elif searchType[-1] == '2':
            customers = models.Customer.objects.filter(customername=request.POST.get('customerName'))
            if not customers:
                if searchType[0] == 'S':
                    return render(request, 'searchAccount.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyAccount.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteAccount.html', {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
                elif searchType[0] == 'E':
                    return render(request, 'depositWithdrawal.html',
                                  {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
            lis = []
            # 可能有同名之人
            for m in range(customers.count()):
                savecustomer = customers[m].savecustomer_set.all()
                chequecustomer = customers[m].chequecustomer_set.all()
                # print(chequecustomer.count())
                for i in savecustomer:
                    save = model_to_dict(i.accountid)
                    save['accounttype'] = '储蓄账户'
                    save['customerid'] = customers[m].customerid
                    lis.append(save)
                for i in chequecustomer:
                    cheque = model_to_dict(i.accountid)
                    cheque['customerid'] = customers[m].customerid
                    cheque['accounttype'] = '支票账户'
                    lis.append(cheque)
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        elif searchType[-1] == '3':
            # 一个账号可能有多个客户
            saves = models.Save.objects.filter(accountid=request.POST.get('accountID'))
            cheques = models.Cheque.objects.filter(accountid=request.POST.get('accountID'))
            # print(request.POST.get('accountID'))
            # print(saves.count())
            # print(cheques.count())
            if not saves and not cheques:
                if searchType[0] == 'S':
                    return render(request, 'searchAccount.html', {"code": json.dumps(400), "msg": json.dumps("账号不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyAccount.html', {"code": json.dumps(400), "msg": json.dumps("账号不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteAccount.html', {"code": json.dumps(400), "msg": json.dumps("账号不存在")})
                elif searchType[0] == 'E':
                    return render(request, 'depositWithdrawal.html',
                                  {"code": json.dumps(400), "msg": json.dumps("账号不存在")})
            # 账户存在一定有客户
            lis = []
            for i in range(saves.count()):
                save = model_to_dict(saves[i])
                save['accounttype'] = '储蓄账户'
                saveCustomer = saves[i].savecustomer_set.all().first()
                save['customerid'] = saveCustomer.customerid.customerid
                lis.append(save)
            for i in range(cheques.count()):
                cheque = model_to_dict(cheques[i])
                cheque['accounttype'] = '支票账户'
                chequeCustomer = cheques[i].chequecustomer_set.all().first()
                cheque['customerid'] = chequeCustomer.customerid.customerid
                lis.append(cheque)
            # print(lis)
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        else:
            bank = models.Banksubbranch.objects.filter(bankname=request.POST.get('bankName'))
            if not bank:
                if searchType[0] == 'S':
                    return render(request, 'searchAccount.html', {"code": json.dumps(400), "msg": json.dumps("支行不存在")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyAccount.html', {"code": json.dumps(400), "msg": json.dumps("支行不存在")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteAccount.html', {"code": json.dumps(400), "msg": json.dumps("支行不存在")})
                elif searchType[0] == 'E':
                    return render(request, 'depositWithdrawal.html',
                                  {"code": json.dumps(400), "msg": json.dumps("支行不存在")})

            saves = bank[0].save_set.all()
            cheques = bank[0].cheque_set.all()
            # print(saves.count())
            # print(cheques.count())
            # 检测是否有账户
            if not saves and not cheques:
                if searchType[0] == 'S':
                    return render(request, 'searchAccount.html', {"code": json.dumps(400), "msg": json.dumps("该支行无账户")})
                elif searchType[0] == 'M':
                    return render(request, 'modifyAccount.html', {"code": json.dumps(400), "msg": json.dumps("该支行无账户")})
                elif searchType[0] == 'D':
                    return render(request, 'deleteAccount.html', {"code": json.dumps(400), "msg": json.dumps("改支行无账户")})
                elif searchType[0] == 'E':
                    return render(request, 'depositWithdrawal.html',
                                  {"code": json.dumps(400), "msg": json.dumps("该支行无账号")})
            # 账户存在一定有客户
            lis = []
            for i in range(saves.count()):
                save = model_to_dict(saves[i])
                save['accounttype'] = '储蓄账户'
                saveCustomer = saves[i].savecustomer_set.all().first()
                save['customerid'] = saveCustomer.customerid.customerid
                lis.append(save)
            for i in range(cheques.count()):
                cheque = model_to_dict(cheques[i])
                cheque['accounttype'] = '支票账户'
                chequeCustomer = cheques[i].chequecustomer_set.all().first()
                cheque['customerid'] = chequeCustomer.customerid.customerid
                lis.append(cheque)
            # print(lis)
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        if searchType[0] == 'S':
            return render(request, 'searchAccount.html', jsonData)
        elif searchType[0] == 'M':
            return render(request, 'modifyAccount.html', jsonData)
        elif searchType[0] == 'D':
            return render(request, 'deleteAccount.html', jsonData)
        elif searchType[0] == 'E':
            return render(request, 'depositWithdrawal.html', jsonData)

    return render(request, 'searchAccount.html', {"msg": msg})


def searchLoadCondition(request):
    code = "400"
    msg = ""
    if request.method == 'POST':
        searchType = request.POST.get('searchType')
        if searchType == '1':
            customers = models.Customer.objects.filter(customerid=request.POST.get('customerID'))
            if not customers:
                # 如果客户不存在
                return render(request, 'searchLoan.html',
                              {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
            loanCustomer = customers[0].customerloan_set.all()
            # print(chequecustomer.count())
            lis = []
            for i in loanCustomer:
                loan = model_to_dict(i.loanid)
                loan['customerid'] = customers[0].customerid
                lis.append(loan)
            # print(lis)
            jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        elif searchType == '2':
            customers = models.Customer.objects.filter(customername=request.POST.get('customerName'))
            if not customers:
                return render(request, 'searchLoan.html',
                              {"code": json.dumps(400), "msg": json.dumps("客户不存在")})
            lis = []
            # 可能有同名之人
            for m in range(customers.count()):
                loancustomer = customers[m].customerloan_set.all()
                # print(chequecustomer.count())
                for i in loancustomer:
                    loan = model_to_dict(i.loanid)
                    loan['customerid'] = customers[m].customerid
                    lis.append(loan)
        elif searchType == '3':
            # 一个账号可能有多个客户
            loans = models.Loan.objects.filter(loanid=request.POST.get('accountID'))
            if not loans:
                return render(request, 'searchLoan.html',
                              {"code": json.dumps(400), "msg": json.dumps("贷款号不存在")})
            # 账户存在一定有客户
            lis = []
            for i in range(loans.count()):
                loan = model_to_dict(loans[i])
                loanCustomer = loans[i].customerloan_set.all().first()
                loan['customerid'] = loanCustomer.customerid.customerid
                lis.append(loan)
            # print(lis)

        else:
            bank = models.Banksubbranch.objects.filter(bankname=request.POST.get('bankName'))
            if not bank:
                return render(request, 'searchLoan.html',
                              {"code": json.dumps(400), "msg": json.dumps("支行不存在")})

            loans = bank[0].loan_set.all()
            # 检测是否有账户
            if not loans:
                return render(request, 'searchLoan.html',
                              {"code": json.dumps(400), "msg": json.dumps("该支行无贷款")})
            # 账户存在一定有客户
            lis = []
            for i in range(loans.count()):
                loan = model_to_dict(loans[i])
                loanCustomer = loans[i].customerloan_set.all().first()
                loan['customerid'] = loanCustomer.customerid.customerid
                lis.append(loan)
            # print(lis)
        for i in range(len(lis)):
            if lis[i]['state'] == '0':
                lis[i]['state'] = "未发放"
            elif lis[i]['state'] == '1':
                lis[i]['state'] = '发放中'
            else:
                lis[i]['state'] = '发放完毕'
        jsonData = {"code": "200", "msg": "查找成功", "data": lis}
        return render(request, 'searchLoan.html', jsonData)
    return render(request, 'searchLoan.html', {"msg": msg})


def searchLoanInfo(request):
    code = "400"
    msg = ""
    if request.method == 'POST':
        searchType = request.POST.get('searchType')
        loans = models.Loan.objects.filter(loanid=request.POST.get('accountID'))
        if not loans:
            if searchType == "delete":
                return render(request, 'deleteLoan.html',
                              {"code": json.dumps(400), "msg": json.dumps("贷款号不存在")})
            elif searchType == "pay":
                return render(request, 'payLoan.html',
                              {"code": json.dumps(400), "msg": json.dumps("贷款号不存在")})
            else:
                return render(request, 'searchLoanInfo.html',
                              {"code": json.dumps(400), "msg": json.dumps("贷款号不存在")})
        # 账户存在一定有客户
        lis = []
        for i in range(loans.count()):
            loan = model_to_dict(loans[i])
            loanCustomer = loans[i].customerloan_set.all().first()
            loan['customerid'] = loanCustomer.customerid.customerid
            lis.append(loan)
        for i in range(len(lis)):
            if lis[i]['state'] == '0':
                lis[i]['state'] = "未发放"
            elif lis[i]['state'] == '1':
                lis[i]['state'] = '发放中'
            else:
                lis[i]['state'] = '发放完毕'
        payments = loans[0].payment_set.all()
        paymentInfo = []
        for payment in payments:
            paymentInfo.append(model_to_dict(payment))
        jsonData = {"code": "200", "msg": "查找成功", "data": lis, "payment": paymentInfo}
        print(jsonData)
        if searchType == "delete":
            return render(request, 'deleteLoan.html', jsonData)
        elif searchType == "pay":
            return render(request, 'payLoan.html', jsonData)
        else:
            return render(request, 'searchLoanInfo.html', jsonData)
    return render(request, 'searchLoanInfo.html', {"msg": msg})


def addAccount(request):
    return render(request, 'addAccount.html')


def addSaveAccount(request):
    if request.method == 'POST':
        customerID = request.POST.get('customerID')
        customerName = request.POST.get('customerName')
        accountID = request.POST.get('accountID')
        bankName = request.POST.get('bankName')
        currency = request.POST.get('currency')
        rate = request.POST.get('rate')
        cash = request.POST.get('cash')
        openDate = request.POST.get('openTime')
        # 判断客户是否存在，信息是否相符
        customer = models.Customer.objects.filter(customerid=customerID)
        if not customer:
            # 如果客户不存在
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("客户不存在")})
        if customer[0].customername != customerName:
            # 信息不符
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("客户信息不符")})
        account = models.Save.objects.filter(accountid=accountID)
        if account:
            # 账号已经存在
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("账号已经存在")})
        bank = models.Banksubbranch.objects.filter(bankname=bankName)
        if not bank:
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("该银行不存在")})
        if customer[0].bankcustomer_set.all().filter(bankname=bankName).filter(accounttype='1'):
            # 在同一家银行不能开设相同类型账户
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("不能在相同银行开设相同类型账户")})
        newSave = models.Save(accountid=accountID, bankname=bank[0], balance=0, opentime=getTime(openDate),
                              visittime=getTime(openDate), rate=float(rate), currency=currency)
        newSave.save()
        newSaveCustomer = models.Savecustomer(customerid=customer[0], accountid=newSave)
        newSaveCustomer.save()
        newSaveInfo = models.Saveinfo(accountid=newSave, visittime=getTime(openDate), businesstype='1',
                                      cash=float(cash), balance=newSave.balance + float(cash))
        newSaveInfo.save()
        newbankCustomer = models.Bankcustomer(bankname=bank[0], customerid=customer[0], accounttype='1')
        newbankCustomer.save()
        return render(request, 'addAccount.html', {"status": json.dumps(200), "message": json.dumps('储蓄账户开设成功')})
    return render(request, 'addAccount.html')


def addChequeAccount(request):
    if request.method == 'POST':
        customerID = request.POST.get('customerID')
        customerName = request.POST.get('customerName')
        accountID = request.POST.get('accountID')
        bankName = request.POST.get('bankName')
        overdraft = request.POST.get('overdraft')
        cash = request.POST.get('cash')
        openDate = request.POST.get('openTime')
        # 判断客户是否存在，信息是否相符
        customer = models.Customer.objects.filter(customerid=customerID)
        if not customer:
            # 如果客户不存在
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("客户不存在")})
        if customer[0].customername != customerName:
            # 信息不符
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("客户信息不符")})
        account = models.Cheque.objects.filter(accountid=accountID)
        if account:
            # 账号已经存在
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("账号已经存在")})
        bank = models.Banksubbranch.objects.filter(bankname=bankName)
        if not bank:
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("该银行不存在")})
        if customer[0].bankcustomer_set.all().filter(bankname=bankName).filter(accounttype='2'):
            # 在同一家银行不能开设相同类型账户
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("不能在相同银行开设相同类型账户")})
        newCheque = models.Cheque(accountid=accountID, bankname=bank[0], balance=0, opentime=getTime(openDate),
                                  visittime=getTime(openDate), overdraft=overdraft)
        newCheque.save()
        newChequeCustomer = models.Chequecustomer(customerid=customer[0], accountid=newCheque)
        newChequeCustomer.save()
        newChequeInfo = models.Chequeinfo(accountid=newCheque, visittime=getTime(openDate), businesstype='1',
                                          cash=float(cash), balance=newCheque.balance + float(cash))
        newChequeInfo.save()
        newbankCustomer = models.Bankcustomer(bankname=bank[0], customerid=customer[0], accounttype='2')
        newbankCustomer.save()
        return render(request, 'addAccount.html', {"status": json.dumps(200), "message": json.dumps('支票账户开设成功')})
    return render(request, 'addAccount.html')


def deleteAccount(request):
    # 删除贷款时需要将bankCustomer的信息一并删除
    # 注意一个账户可能被多人共享
    if request.method == "POST":
        # customer = models.Customer.objects.get(customerid=request.POST.get("customerID"))
        bank = models.Banksubbranch.objects.get(bankname=request.POST.get("bankName"))
        if request.POST.get('accountType') == '储蓄账户':
            accountID = request.POST.get('accountID')
            save = models.Save.objects.get(accountid=accountID)
            try:
                saveCustomers = save.savecustomer_set.all()
                for saveCustomer in saveCustomers:
                    customer = saveCustomer.customerid
                    bankCustomer = models.Bankcustomer.objects.get(bankname=bank, customerid=customer, accounttype='1')
                    bankCustomer.delete()
                save.delete()
            except:
                return render(request, 'deleteAccount.html',
                              {"code": json.dumps(400), "msg": json.dumps("销户失败")})
            return render(request, 'deleteAccount.html', {"code": json.dumps(200), "msg": json.dumps("销户成功")})
        else:
            accountID = request.POST.get('accountID')
            cheque = models.Cheque.objects.get(accountid=accountID)
            try:

                chequeCustomers = cheque.chequecustomer_set.all()
                for chequeCustomer in chequeCustomers:
                    customer = chequeCustomer.customerid
                    bankCustomer = models.Bankcustomer.objects.get(bankname=bank, customerid=customer, accounttype='2')
                    bankCustomer.delete()
                cheque.delete()
            except:
                return render(request, 'deleteAccount.html',
                              {"code": json.dumps(400), "msg": json.dumps("销户失败")})
            return render(request, 'deleteAccount.html', {"code": json.dumps(200), "msg": json.dumps("销户成功")})
    return render(request, 'deleteAccount.html')


def modifyAccount(request):
    if request.method == "POST":
        if request.POST.get('accountType') == '储蓄账户':
            accountID = request.POST.get('accountID')
            opendate = request.POST.get('openTime')
            vistdate = request.POST.get('visitTime')
            rate = request.POST.get("rate")
            currency = request.POST.get("currency")
            save = models.Save.objects.get(accountid=accountID)
            save.rate = rate
            save.currency = currency
            save.opentime = getTime(opendate)
            save.visittime = getTime(vistdate)
            save.save()
            return render(request, 'modifyAccount.html', {"code": json.dumps(200), "msg": json.dumps("储蓄账户信息修改成功")})
        else:
            accountID = request.POST.get('accountID')
            opendate = request.POST.get('openTime')
            vistdate = request.POST.get('visitTime')
            overdraft = request.POST.get("overdraft")
            cheque = models.Cheque.objects.get(accountid=accountID)
            cheque.overdraft = overdraft
            cheque.opentime = getTime(opendate)
            cheque.visittime = getTime(vistdate)
            cheque.save()
            return render(request, 'modifyAccount.html', {"code": json.dumps(200), "msg": json.dumps("支票账户信息修改成功")})
    return render(request, 'modifyAccount.html')


def depositWithdrawal(request):
    if request.method == 'POST':
        cash = Decimal(float(request.POST.get('cash')))
        if request.POST.get('accountType') == "储蓄账户":
            save = models.Save.objects.get(accountid=request.POST.get('accountID'))
            if request.POST.get('isDeposit') == "存款":
                newsaveinfo = models.Saveinfo(accountid=save, visittime=getTime(request.POST.get('now')),
                                              businesstype='1', cash=cash,
                                              balance=(save.balance + cash))
                newsaveinfo.save()
                return render(request, 'depositWithdrawal.html',
                              {"code": json.dumps(200), "msg": json.dumps("储蓄账户-存款成功")})
            else:
                print(cash > save.balance)
                if cash > save.balance:
                    return render(request, 'depositWithdrawal.html',
                                  {"code": json.dumps(400), "msg": json.dumps("储蓄账户-账户余额不足")})
                newsaveinfo = models.Saveinfo(accountid=save, visittime=getTime(request.POST.get('now')),
                                              businesstype='0', cash=cash,
                                              balance=(save.balance - cash))
                newsaveinfo.save()
                return render(request, 'depositWithdrawal.html',
                              {"code": json.dumps(200), "msg": json.dumps("储蓄账户-取款成功")})
        else:
            cheque = models.Cheque.objects.get(accountid=request.POST.get('accountID'))
            if request.POST.get('isDeposit') == "存款":
                newchequeinfo = models.Chequeinfo(accountid=cheque, visittime=getTime(request.POST.get('now')),
                                                  businesstype='1', cash=cash,
                                                  balance=(cheque.balance + cash))
                newchequeinfo.save()
                return render(request, 'depositWithdrawal.html',
                              {"code": json.dumps(200), "msg": json.dumps("支票账户-存款成功")})

            else:
                if cash > cheque.balance + cheque.overdraft:
                    return render(request, 'depositWithdrawal.html',
                                  {"code": json.dumps(400), "msg": json.dumps("支票账户-账户额度不足")})
                newchequeinfo = models.Chequeinfo(accountid=cheque, visittime=getTime(request.POST.get('now')),
                                                  businesstype='0', cash=cash,
                                                  balance=(cheque.balance - cash))
                newchequeinfo.save()
                return render(request, 'depositWithdrawal.html',
                              {"code": json.dumps(200), "msg": json.dumps("支票账户-取款成功")})
    return render(request, 'depositWithdrawal.html')


def searchAccount(request):
    return render(request, 'searchAccount.html')


def queryToList(datas, msg="查找成功"):
    lis = []
    try:
        datasCount = datas.count()
    except:
        lis.append(model_to_dict(datas))
        jsonData = {"code": "200", "msg": msg, "data": lis}
        return jsonData
    for i in range(datasCount):
        lis.append(model_to_dict(datas[i]))
    jsonData = {"code": "200", "msg": msg, "data": lis}
    # jsonData = json.dumps(jsonData,cls=DateEncoder)
    return jsonData


def searchCustomerAll(request):
    customers = models.Customer.objects.all()
    customersCount = customers.count()
    lis = []
    for i in range(customersCount):
        lis.append(model_to_dict(customers[i]))
    data = {"code": 0, "msg": "", "count": 1000, "data": lis}
    # print(data)
    return HttpResponse(json.dumps(data, cls=DateEncoder), content_type="application/json")


def searchSaveAccountAll(request):
    saves = models.Save.objects.all()
    savesCount = saves.count()
    lis = []
    for i in range(savesCount):
        lis.append(model_to_dict(saves[i]))
    data = {"code": 0, "msg": "", "count": 1000, "data": lis}
    return HttpResponse(json.dumps(data, cls=DateEncoder), content_type="application/json")


def searchChequeAccountAll(request):
    cheques = models.Cheque.objects.all()
    chequesCount = cheques.count()
    lis = []
    for i in range(chequesCount):
        lis.append(model_to_dict(cheques[i]))
    data = {"code": 0, "msg": "", "count": 1000, "data": lis}
    return HttpResponse(json.dumps(data, cls=DateEncoder), content_type="application/json")


def searchLoanAll(request):
    loans = models.Loan.objects.all()
    loansCount = loans.count()
    lis = []
    for i in range(loansCount):
        lis.append(model_to_dict(loans[i]))
        if lis[i]['state'] == '0':
            lis[i]['state'] = "未发放"
        elif lis[i]['state'] == '1':
            lis[i]['state'] = '发放中'
        else:
            lis[i]['state'] = '发放完毕'
    data = {"code": 0, "msg": "", "count": 1000, "data": lis}
    return HttpResponse(json.dumps(data, cls=DateEncoder), content_type="application/json")


def addLoan(request):
    if request.method == 'POST':
        customerID = request.POST.get('customerID')
        customerName = request.POST.get('customerName')
        loanID = request.POST.get('loanID')
        bankName = request.POST.get('bankName')
        sum = request.POST.get('sum')
        times = request.POST.get('times')
        createDate = request.POST.get('createTime')
        # 判断客户是否存在，信息是否相符
        customer = models.Customer.objects.filter(customerid=customerID)
        if not customer:
            # 如果客户不存在
            return render(request, 'addLoan.html',
                          {"status": json.dumps(400), "message": json.dumps("客户不存在")})
        if customer[0].customername != customerName:
            # 信息不符
            return render(request, 'addLoan.html',
                          {"status": json.dumps(400), "message": json.dumps("客户信息不符")})
        loan = models.Loan.objects.filter(loanid=loanID)
        if loan:
            # 账号已经存在
            return render(request, 'addLoan.html',
                          {"status": json.dumps(400), "message": json.dumps("账号已经存在")})
        bank = models.Banksubbranch.objects.filter(bankname=bankName)
        if not bank:
            # 如果银行不存在
            return render(request, 'addLoan.html',
                          {"status": json.dumps(400), "message": json.dumps("该银行不存在")})
        if customer[0].bankcustomer_set.all().filter(bankname=bankName).filter(accounttype='3'):
            # 在同一家银行不能开设相同类型账户
            return render(request, 'addAccount.html',
                          {"status": json.dumps(400), "message": json.dumps("不能在相同银行开设相同类型账户")})
        newLoan = models.Loan(loanid=loanID, bankname=bank[0], sum=float(sum), times=int(times),
                              paysum=0, state='0')
        newLoan.save()
        newCustomerLoan = models.Customerloan(customerid=customer[0], loanid=newLoan)
        newCustomerLoan.save()
        newPayment = models.Payment(loanid=newLoan, paytime=getTime(createDate), cash=0)
        newPayment.save()
        newbankCustomer = models.Bankcustomer(bankname=bank[0], customerid=customer[0], accounttype='3')
        newbankCustomer.save()
        return render(request, 'addLoan.html', {"status": json.dumps(200), "message": json.dumps('贷款办理成功')})
    return render(request, "addLoan.html")


def deleteLoan(request):
    # 删除贷款时删除相应的bankCustomer信息
    if request.method == "POST":
        loanID = request.POST.get('accountID')
        loan = models.Loan.objects.get(loanid=loanID)
        if loan.state == '1':
            return render(request, 'deleteLoan.html', {"code": json.dumps(400), "msg": json.dumps("贷款发放中，不能删除")})
        try:

            bank = loan.bankname
            customerloans = loan.customerloan_set.all()
            for customerloan in customerloans:
                customer = customerloan.customerid
                bankCustomer = models.Bankcustomer.objects.get(bankname=bank, customerid=customer, accounttype="3")
                bankCustomer.delete()
            loan.delete()
        except:
            return render(request, 'deleteLoan.html', {"code": json.dumps(400), "msg": json.dumps("贷款删除失败")})
        return render(request, 'deleteLoan.html', {"code": json.dumps(200), "msg": json.dumps("贷款删除成功")})

    return render(request, 'deleteLoan.html')


def payLoan(request):
    # 贷款发放总额不能超过贷款额
    if request.method == "POST":
        loanid = request.POST.get('accountID')
        cash = request.POST.get('cash')
        nowdate = request.POST.get('now')
        newPayment = models.Payment(loanid_id=loanid, paytime=getTime(nowdate), cash=float(cash))
        try:
            newPayment.save()
        except:
            return render(request, 'payLoan.html', {"code": json.dumps(400), "msg": json.dumps("贷款发生失败-发放总额超过贷款额")})
        return render(request, 'payLoan.html', {"code": json.dumps(200), "msg": json.dumps("贷款发放完成")})
    return render(request, 'payLoan.html')


def searchLoan(request):
    return render(request, 'searchLoan.html')


def loanStatistics(request):
    if request.method == "POST":
        searchType = request.POST.get('searchType')
        year1 = int(request.POST.get("year1"))
        year2 = int(request.POST.get("year2"))
        year3 = int(request.POST.get("year3"))
        banks = models.Banksubbranch.objects.all()
        user = [['bankName']]
        cash = [['bankName']]
        userBusiness = [['bankName']]
        cashBusiness = [['bankName']]
        if searchType == "S1":
            if year2 < year1:
                tmp = year1
                year1 = year2
                year2 = tmp
            for i in range(1, abs(year2 - year1) + 2):
                user.append([str(year1 + i - 1) + '年'])
                cash.append([str(year1 + i - 1) + '年'])
                userBusiness.append([str(year1 + i - 1) + '年'])
                cashBusiness.append([str(year1 + i - 1) + '年'])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                loans = bank.loan_set.all()
                for i in range(1, abs(year2 - year1) + 2):
                    user[i].append(loans.filter(payment__cash=0,payment__paytime__lt=str2time(year1 + i)).__len__())
                    userBusiness[i].append(
                        loans.filter(payment__cash=0,payment__paytime__lt=str2time(year1 + i), payment__paytime__gte=str2time(year1 + i - 1)).__len__())
                    sum = 0
                    sum2 = 0
                    for loan in loans:
                        loanInfos = loan.payment_set.all().filter(paytime__lt=str2time(year1 + i))
                        if loanInfos:
                            sum += loanInfos.aggregate(Sum("cash"))["cash__sum"]

                        loanInfos = loan.payment_set.all().filter(paytime__lt=str2time(year1 + i),
                                                                   paytime__gte=str2time(year1 + i - 1))
                        if loanInfos:
                            sum2 += loanInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[i].append(float(sum))
                    cashBusiness[i].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "loanStatistics.html",
                          {"code": json.dumps(200), "msg":json.dumps("统计结果返回成功"), "user": user, "cash": cash, "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
        elif searchType == "S2":
            for i in range(1, 13):
                user.append([str(i) + '月份'])
                cash.append([str(i) + '月份'])
                userBusiness.append([str(i) + '月份'])
                cashBusiness.append([str(i) + '月份'])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                loans = bank.loan_set.all()
                for i in range(1, 13):
                    user[i].append(loans.filter(payment__cash=0, payment__paytime__lt=str2time(year3,i+1)).__len__())
                    userBusiness[i].append(
                        loans.filter(payment__cash=0, payment__paytime__lt=str2time(year3,i+1),
                                     payment__paytime__gte=str2time(year3 ,i)).__len__())
                    sum = 0
                    sum2 = 0
                    for loan in loans:
                        loanInfos = loan.payment_set.all().filter(paytime__lt=str2time(year3,i+1))
                        if loanInfos:
                            sum += loanInfos.aggregate(Sum("cash"))["cash__sum"]

                        loanInfos = loan.payment_set.all().filter(paytime__lt=str2time(year3,i+1),
                                                                  paytime__gte=str2time(year3, i))
                        if loanInfos:
                            sum2 += loanInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[i].append(float(sum))
                    cashBusiness[i].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "loanStatistics.html",
                          {"code": json.dumps(200), "msg": json.dumps("统计结果返回成功"), "user": user, "cash": cash,
                           "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
        else:
            for i in range(1, 5):
                user.append([chinese[i]])
                cash.append([chinese[i]])
                userBusiness.append([chinese[i]])
                cashBusiness.append([chinese[i]])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                loans = bank.loan_set.all()
                for i in range(3, 13, 3):
                    user[int(i/3)].append(loans.filter(payment__cash=0, payment__paytime__lt=str2time(year3, i + 1)).__len__())
                    userBusiness[int(i/3)].append(
                        loans.filter(payment__cash=0, payment__paytime__lt=str2time(year3, i + 1),
                                     payment__paytime__gte=str2time(year3, i-2)).__len__())
                    sum = 0
                    sum2 = 0
                    for loan in loans:
                        loanInfos = loan.payment_set.all().filter(paytime__lt=str2time(year3, i + 1))
                        if loanInfos:
                            sum += loanInfos.aggregate(Sum("cash"))["cash__sum"]

                        loanInfos = loan.payment_set.all().filter(paytime__lt=str2time(year3, i + 1),
                                                                  paytime__gte=str2time(year3, i-2))
                        if loanInfos:
                            sum2 += loanInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[int(i / 3)].append(float(sum))
                    cashBusiness[int(i / 3)].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "loanStatistics.html",
                          {"code": json.dumps(200), "msg": json.dumps("统计结果返回成功"), "user": user, "cash": cash,
                           "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
    return render(request, 'loanStatistics.html')


def str2time(year, month='1', day='1'):
    if int(month) > 12:
        return utc.localize(datetime.datetime.strptime(str(int(year) + 1) + '-' + '1' + '-' + '1', '%Y-%m-%d'))
    return utc.localize(datetime.datetime.strptime(str(year) + '-' + str(month) + '-' + str(day), '%Y-%m-%d'))


def saveStatistics(request):
    if request.method == "POST":
        searchType = request.POST.get('searchType')
        year1 = int(request.POST.get("year1"))
        year2 = int(request.POST.get("year2"))
        year3 = int(request.POST.get("year3"))
        banks = models.Banksubbranch.objects.all()
        user = [['bankName']]
        cash = [['bankName']]
        userBusiness = [['bankName']]
        cashBusiness = [['bankName']]
        if searchType == "S1":
            if year2 < year1:
                tmp = year1
                year1 = year2
                year2 = tmp
            for i in range(1, abs(year2 - year1) + 2):
                user.append([str(year1 + i - 1) + '年'])
                cash.append([str(year1 + i - 1) + '年'])
                userBusiness.append([str(year1 + i - 1) + '年'])
                cashBusiness.append([str(year1 + i - 1) + '年'])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                saves = bank.save_set.all()
                for i in range(1, abs(year2 - year1) + 2):
                    user[i].append(saves.filter(opentime__lt=str2time(year1 + i)).__len__())
                    userBusiness[i].append(
                        saves.filter(opentime__lt=str2time(year1 + i), opentime__gte=str2time(year1 + i - 1)).__len__())
                    sum = 0
                    sum2 = 0
                    for save in saves:
                        saveInfos = save.saveinfo_set.all().filter(visittime__lt=str2time(year1 + i)).order_by(
                            '-visittime')
                        saveInfo = saveInfos.first()
                        if saveInfo:
                            sum += saveInfo.balance

                        saveInfos = save.saveinfo_set.all().filter(visittime__lt=str2time(year1 + i),
                                                                   visittime__gte=str2time(year1 + i - 1))
                        if saveInfos:
                            sum2 += saveInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[i].append(float(sum))
                    cashBusiness[i].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "saveStatistics.html",
                          {"code": json.dumps(200), "msg":json.dumps("统计结果返回成功"), "user": user, "cash": cash, "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
        elif searchType == "S2":
            for i in range(1, 13):
                user.append([str(i) + '月份'])
                cash.append([str(i) + '月份'])
                userBusiness.append([str(i) + '月份'])
                cashBusiness.append([str(i) + '月份'])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                saves = bank.save_set.all()
                for i in range(1, 13):
                    user[i].append(saves.filter(opentime__lt=str2time(year3, i + 1)).__len__())
                    userBusiness[i].append(
                        saves.filter(opentime__lt=str2time(year3, i + 1), opentime__gte=str2time(year3, i)).__len__())
                    sum = 0
                    sum2 = 0
                    for save in saves:
                        saveInfos = save.saveinfo_set.all().filter(visittime__lt=str2time(year3, i+1)).order_by(
                            '-visittime')
                        saveInfo = saveInfos.first()
                        if saveInfo:
                            sum += saveInfo.balance

                        saveInfos = save.saveinfo_set.all().filter(visittime__lt=str2time(year3, i + 1),
                                                                   visittime__gte=str2time(year3, i))
                        if saveInfos:
                            sum2 += saveInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[i].append(float(sum))
                    cashBusiness[i].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "saveStatistics.html",
                          {"code": json.dumps(200), "msg": json.dumps("统计结果返回成功"), "user": user, "cash": cash,
                           "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
        else:
            for i in range(1, 5):
                user.append([chinese[i]])
                cash.append([chinese[i]])
                userBusiness.append([chinese[i]])
                cashBusiness.append([chinese[i]])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                saves = bank.save_set.all()
                for i in range(3, 13, 3):
                    user[int(i / 3)].append(saves.filter(opentime__lt=str2time(year3, i + 1)).__len__())
                    userBusiness[int(i / 3)].append(saves.filter(opentime__lt=str2time(year3, i + 1),
                                                                 opentime__gte=str2time(year3, i - 2)).__len__())
                    sum = 0
                    sum2 = 0
                    for save in saves:
                        saveInfos = save.saveinfo_set.all().filter(visittime__lt=str2time(year3, i + 1)).order_by(
                            '-visittime')
                        saveInfo = saveInfos.first()
                        if saveInfo:
                            sum += saveInfo.balance

                        saveInfos = save.saveinfo_set.all().filter(visittime__lt=str2time(year3, i + 1),
                                                                   visittime__gte=str2time(year3, i - 2))
                        if saveInfos:
                            sum2 += saveInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[int(i / 3)].append(float(sum))
                    cashBusiness[int(i / 3)].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "saveStatistics.html",
                          {"code": json.dumps(200), "msg": json.dumps("统计结果返回成功"), "user": user, "cash": cash,
                           "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
    return render(request, 'saveStatistics.html')


def chequeStatistics(request):
    if request.method == "POST":
        searchType = request.POST.get('searchType')
        year1 = int(request.POST.get("year1"))
        year2 = int(request.POST.get("year2"))
        year3 = int(request.POST.get("year3"))
        banks = models.Banksubbranch.objects.all()
        user = [['bankName']]
        cash = [['bankName']]
        userBusiness = [['bankName']]
        cashBusiness = [['bankName']]
        if searchType == "S1":
            if year2 < year1:
                tmp = year1
                year1 = year2
                year2 = tmp
            for i in range(1, abs(year2 - year1) + 2):
                user.append([str(year1 + i - 1) + '年'])
                cash.append([str(year1 + i - 1) + '年'])
                userBusiness.append([str(year1 + i - 1) + '年'])
                cashBusiness.append([str(year1 + i - 1) + '年'])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                cheques = bank.cheque_set.all()
                for i in range(1, abs(year2 - year1) + 2):
                    user[i].append(cheques.filter(opentime__lt=str2time(year1 + i)).__len__())
                    userBusiness[i].append(
                        cheques.filter(opentime__lt=str2time(year1 + i), opentime__gte=str2time(year1 + i - 1)).__len__())
                    sum = 0
                    sum2 = 0
                    for cheque in cheques:
                        chequeInfos = cheque.chequeinfo_set.all().filter(visittime__lt=str2time(year1 + i)).order_by(
                            '-visittime')
                        chequeInfo = chequeInfos.first()
                        if chequeInfo:
                            sum += chequeInfo.balance

                        chequeInfos = cheque.chequeinfo_set.all().filter(visittime__lt=str2time(year1 + i),
                                                                   visittime__gte=str2time(year1 + i - 1))
                        if chequeInfos:
                            sum2 += chequeInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[i].append(float(sum))
                    cashBusiness[i].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "chequeStatistics.html",
                          {"code": json.dumps(200), "msg": json.dumps("统计结果返回成功"), "user": user, "cash": cash,
                           "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
        elif searchType == "S2":
            for i in range(1, 13):
                user.append([str(i) + '月份'])
                cash.append([str(i) + '月份'])
                userBusiness.append([str(i) + '月份'])
                cashBusiness.append([str(i) + '月份'])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                cheques = bank.cheque_set.all()
                for i in range(1, 13):
                    user[i].append(cheques.filter(opentime__lt=str2time(year3, i + 1)).__len__())
                    userBusiness[i].append(
                        cheques.filter(opentime__lt=str2time(year3, i + 1), opentime__gte=str2time(year3, i)).__len__())
                    sum = 0
                    sum2 = 0
                    for cheque in cheques:
                        chequeInfos = cheque.chequeinfo_set.all().filter(visittime__lt=str2time(year3, i+1)).order_by(
                            '-visittime')
                        chequeInfo = chequeInfos.first()
                        if chequeInfo:
                            sum += chequeInfo.balance

                        chequeInfos = cheque.chequeinfo_set.all().filter(visittime__lt=str2time(year3, i + 1),
                                                                   visittime__gte=str2time(year3, i))
                        if chequeInfos:
                            sum2 += chequeInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[i].append(float(sum))
                    cashBusiness[i].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "chequeStatistics.html",
                          {"code": json.dumps(200), "msg": json.dumps("统计结果返回成功"), "user": user, "cash": cash,
                           "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
        else:
            for i in range(1, 5):
                user.append([chinese[i]])
                cash.append([chinese[i]])
                userBusiness.append([chinese[i]])
                cashBusiness.append([chinese[i]])
            for bank in banks:
                # 对每个银行每年用户和金额进行统计
                user[0].append(bank.bankname)
                cash[0].append(bank.bankname)
                userBusiness[0].append(bank.bankname)
                cashBusiness[0].append(bank.bankname)
                # 对每年
                cheques = bank.cheque_set.all()
                for i in range(3, 13, 3):
                    user[int(i / 3)].append(cheques.filter(opentime__lt=str2time(year3, i + 1)).__len__())
                    userBusiness[int(i / 3)].append(cheques.filter(opentime__lt=str2time(year3, i + 1),
                                                                 opentime__gte=str2time(year3, i - 2)).__len__())
                    sum = 0
                    sum2 = 0
                    for cheque in cheques:
                        chequeInfos = cheque.chequeinfo_set.all().filter(visittime__lt=str2time(year3, i + 1)).order_by(
                            '-visittime')
                        chequeInfo = chequeInfos.first()
                        if chequeInfo:
                            sum += chequeInfo.balance

                        chequeInfos = cheque.chequeinfo_set.all().filter(visittime__lt=str2time(year3, i + 1),
                                                                   visittime__gte=str2time(year3, i - 2))
                        if chequeInfos:
                            sum2 += chequeInfos.aggregate(Sum("cash"))["cash__sum"]
                        # print(sum)
                    cash[int(i / 3)].append(float(sum))
                    cashBusiness[int(i / 3)].append(float(sum2))
            # print(user)
            # print(cash)
            # print(userBusiness)
            # print(cashBusiness)
            return render(request, "chequeStatistics.html",
                          {"code": json.dumps(200), "msg": json.dumps("统计结果返回成功"), "user": user, "cash": cash,
                           "userBusiness": userBusiness,
                           "cashBusiness": cashBusiness})
    return render(request, 'chequeStatistics.html')


