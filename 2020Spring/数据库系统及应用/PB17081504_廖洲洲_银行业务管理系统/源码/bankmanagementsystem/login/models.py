from django.db import models

# Create your models here.

class User(models.Model):
    username = models.CharField(max_length = 255, unique = True)
    password = models.CharField(max_length = 256)

    def __str__(self):
        return self.username


class Banksubbranch(models.Model):
    bankname = models.CharField(db_column='bankName', primary_key=True, max_length=50)  # Field name made lowercase.
    city = models.CharField(max_length=50, blank=True, null=True)
    property = models.DecimalField(max_digits=16, decimal_places=2)

    class Meta:
        db_table = 'banksubbranch'

class Department(models.Model):
    departmentnum = models.CharField(db_column='departmentNum', primary_key=True, max_length=18)  # Field name made lowercase.
    bankname = models.ForeignKey(Banksubbranch, models.CASCADE, db_column='bankName', blank=True, null=True)  # Field name made lowercase.
    manager = models.CharField(max_length=18, blank=True, null=True)
    departmentname = models.CharField(db_column='departmentName', max_length=18, blank=True, null=True)  # Field name made lowercase.
    type = models.IntegerField(blank=True, null=True)

    class Meta:
        db_table = 'department'

class Staff(models.Model):
    staffid = models.CharField(db_column='staffID', primary_key=True, max_length=18)  # Field name made lowercase.
    departmentnum = models.ForeignKey(Department, models.CASCADE, db_column='departmentNum', blank=True, null=True)  # Field name made lowercase.
    startdate = models.DateField(db_column='startDate')  # Field name made lowercase.
    address = models.CharField(max_length=50, blank=True, null=True)
    staffname = models.CharField(db_column='staffName', max_length=18)  # Field name made lowercase.
    phone = models.CharField(max_length=11, blank=True, null=True)

    class Meta:
        db_table = 'staff'

class Customer(models.Model):
    customerid = models.CharField(db_column='customerID', primary_key=True, max_length=18)  # Field name made lowercase.
    staffid = models.ForeignKey(Staff, models.DO_NOTHING, db_column='staffID', blank=True, null=True)  # Field name made lowercase.
    address = models.CharField(max_length=50, blank=True, null=True)
    customername = models.CharField(db_column='customerName', max_length=18)  # Field name made lowercase.
    createtime = models.DateTimeField(db_column='createTime')  # Field name made lowercase.
    phone = models.CharField(max_length=11, blank=True, null=True)
    linkmanemail = models.CharField(db_column='linkmanEmail', max_length=64)  # Field name made lowercase.
    linkmanname = models.CharField(db_column='linkmanName', max_length=18)  # Field name made lowercase.
    linkmanphone = models.CharField(db_column='linkmanPhone', max_length=11, blank=True, null=True)  # Field name made lowercase.
    linkmanrelationship = models.CharField(db_column='linkmanRelationship', max_length=64, blank=True, null=True)  # Field name made lowercase.

    class Meta:
        db_table = 'customer'




class Bankcustomer(models.Model):
    bankname = models.ForeignKey(Banksubbranch, models.PROTECT, db_column='bankName')  # Field name made lowercase.
    customerid = models.ForeignKey(Customer, models.CASCADE, db_column='customerID')  # Field name made lowercase.
    accounttype = models.CharField(db_column='accountType', max_length=1)  # Field name made lowercase.

    class Meta:
        db_table = 'bankcustomer'
        unique_together = (('bankname', 'customerid', 'accounttype'),)





class Cheque(models.Model):
    accountid = models.CharField(db_column='accountID', primary_key=True, max_length=36)  # Field name made lowercase.
    bankname = models.ForeignKey(Banksubbranch, models.PROTECT, db_column='bankName')  # Field name made lowercase.
    balance = models.DecimalField(max_digits=16, decimal_places=2, blank=True, null=True)
    opentime = models.DateTimeField(db_column='openTime', blank=True, null=True)  # Field name made lowercase.
    visittime = models.DateTimeField(db_column='visitTime', blank=True, null=True)  # Field name made lowercase.
    overdraft = models.DecimalField(max_digits=16, decimal_places=2)

    class Meta:
        db_table = 'cheque'


class Chequecustomer(models.Model):
    customerid = models.ForeignKey(Customer, models.PROTECT, db_column='customerID')  # Field name made lowercase.
    accountid = models.ForeignKey(Cheque, models.CASCADE, db_column='accountID')  # Field name made lowercase.

    class Meta:
        db_table = 'chequecustomer'
        unique_together = (('customerid', 'accountid'),)


class Chequeinfo(models.Model):
    accountid = models.ForeignKey(Cheque, models.CASCADE, db_column='accountID')  # Field name made lowercase.
    businesstype = models.CharField(db_column='businessType', max_length=1)  # Field name made lowercase.
    visittime = models.DateTimeField(db_column='visitTime')  # Field name made lowercase.
    cash = models.DecimalField(max_digits=16, decimal_places=2)
    balance = models.DecimalField(max_digits=16, decimal_places=2)

    class Meta:
        db_table = 'chequeinfo'
        unique_together = (('accountid', 'visittime'),)


class Loan(models.Model):
    loanid = models.CharField(db_column='loanID', primary_key=True, max_length=18)  # Field name made lowercase.
    bankname = models.ForeignKey(Banksubbranch, models.PROTECT, db_column='bankName')  # Field name made lowercase.
    sum = models.DecimalField(max_digits=16, decimal_places=2)
    times = models.IntegerField(blank=True, null=True)
    paysum = models.DecimalField(db_column='paySum', max_digits=16, decimal_places=2)  # Field name made lowercase.
    state = models.CharField(max_length=1, blank=True, null=True)

    class Meta:
        db_table = 'loan'


class Customerloan(models.Model):
    loanid = models.ForeignKey(Loan, models.CASCADE, db_column='loanID')  # Field name made lowercase.
    customerid = models.ForeignKey(Customer, models.PROTECT, db_column='customerID')  # Field name made lowercase.

    class Meta:
        db_table = 'customerloan'
        unique_together = (('loanid', 'customerid'),)







class Payment(models.Model):
    loanid = models.ForeignKey(Loan, models.CASCADE, db_column='loanID')  # Field name made lowercase.
    paytime = models.DateTimeField(db_column='payTime')  # Field name made lowercase.
    cash = models.DecimalField(max_digits=16, decimal_places=2)

    class Meta:
        db_table = 'payment'
        unique_together = (('loanid', 'paytime'),)


class Save(models.Model):
    accountid = models.CharField(db_column='accountID', primary_key=True, max_length=36)  # Field name made lowercase.
    bankname = models.ForeignKey(Banksubbranch, models.PROTECT, db_column='bankName')  # Field name made lowercase.
    balance = models.DecimalField(max_digits=16, decimal_places=2, blank=True, null=True)
    opentime = models.DateTimeField(db_column='openTime', blank=True, null=True)  # Field name made lowercase.
    visittime = models.DateTimeField(db_column='visitTime', blank=True, null=True)  # Field name made lowercase.
    rate = models.FloatField()
    currency = models.CharField(max_length=24)

    class Meta:
        db_table = 'save'


class Savecustomer(models.Model):
    customerid = models.ForeignKey(Customer, models.PROTECT, db_column='customerID')  # Field name made lowercase.
    accountid = models.ForeignKey(Save, models.CASCADE, db_column='accountID')  # Field name made lowercase.

    class Meta:
        db_table = 'savecustomer'
        unique_together = (('customerid', 'accountid'),)


class Saveinfo(models.Model):
    accountid = models.ForeignKey(Save, models.CASCADE, db_column='accountID')  # Field name made lowercase.
    businesstype = models.CharField(db_column='businessType', max_length=1)  # Field name made lowercase.
    visittime = models.DateTimeField(db_column='visitTime')  # Field name made lowercase.
    cash = models.DecimalField(max_digits=16, decimal_places=2)
    balance = models.DecimalField(max_digits=16, decimal_places=2)

    class Meta:
        db_table = 'saveinfo'
        unique_together = (('accountid', 'visittime'),)

