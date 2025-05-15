from django.db.models import Model
from django.contrib import admin
from . import models

admin.site.register(models.Client)
admin.site.register(models.Employee)
admin.site.register(models.Service)
admin.site.register(models.Device)
admin.site.register(models.Order)
admin.site.register(models.SparePart)
