from django.contrib import admin
from . import models

# Register your models here.

admin.site.register(models.User)
admin.site.register(models.Cheque)
admin.site.register(models.Loan)
admin.site.register(models.Banksubbranch)
admin.site.register(models.Bankcustomer)
admin.site.register(models.Department)
admin.site.register(models.Payment)
admin.site.register(models.Save)
admin.site.register(models.Staff)
admin.site.register(models.Chequeinfo)
admin.site.register(models.Saveinfo)
admin.site.register(models.Savecustomer)
admin.site.register(models.Customerloan)
admin.site.register(models.Chequecustomer)
admin.site.register(models.Customer)