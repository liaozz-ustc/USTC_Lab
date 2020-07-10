"""register_login_interface URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from login import views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('user/login/', views.login),
    path('index/', views.index),
    path('user/signup/', views.signup),
    path('user/logout/', views.logout),
    path('customer/add/',views.addCustomer),
    path('customer/delete/',views.deleteCustomer),
    path('customer/modify/',views.modifyCustomer),
    path('customer/search/',views.searchCustomer),
    path('customer/search/all/',views.searchCustomerAll),
    path('customer/search/condition/',views.searchCustomerCondition),
    path('account/add/',views.addAccount),
    path('account/add/save',views.addSaveAccount),
    path('account/add/cheque',views.addChequeAccount),
    path('account/search/save/all/',views.searchSaveAccountAll),
    path('account/search/cheque/all/',views.searchChequeAccountAll),
    path('account/search/condition/',views.searchAccountCondition),
    path('account/delete/',views.deleteAccount),
    path('account/modify/',views.modifyAccount),
    path('account/deposit_withdrawal/',views.depositWithdrawal),
    path('account/search/',views.searchAccount),
    path('loan/add/',views.addLoan),
    path('loan/delete/',views.deleteLoan),
    path('loan/pay/',views.payLoan),
    path('loan/search/',views.searchLoan),
    path('loan/search/info/',views.searchLoanInfo),
    path('loan/search/all/',views.searchLoanAll),
    path('loan/search/condition/',views.searchLoadCondition),
    path('loan/statistics/',views.loanStatistics),
    path('save/statistics/',views.saveStatistics),
    path('cheque/statistics/',views.chequeStatistics)
]
