
import datetime
import django
from django import forms
from django.forms import ModelForm, ValidationError
from django import forms
from django.utils.timezone import now

from news import models
from .models import Employee, Order, OrderService, OrderSpareParts



class AddServiceForm(ModelForm):
    """Add service to an order"""

    def __init__(self, *args, **kvargs):
        self.order = kvargs.pop('order', None)
        super(ModelForm, self).__init__(*args, **kvargs)

    class Meta:
        model = OrderService
        fields = ("service", "number")

    def clean_service(self):
        service = self.cleaned_data['service']
        if OrderService.objects.filter(order=self.order).filter(service=service).exists():
            raise ValidationError("This service is already added for order")
        return service

    def save(self, commit=True):
        order_to_service = super().save(commit=False)
        order_to_service.order = self.order

        if commit:
            order_to_service.save()
        return order_to_service

class AddSparePartForm(ModelForm):
    """Add spare part to an order"""

    def __init__(self, *args, **kvargs):
        self.order = kvargs.pop('order', None)
        super(ModelForm, self).__init__(*args, **kvargs)

    class Meta:
        model = OrderSpareParts
        fields = ("spare_part", "number")

    def clean_service(self):
        spare_part = self.cleaned_data['spare_part']
        if OrderSpareParts.objects.filter(order=self.order).filter(spare_part=spare_part).exists():
            raise ValidationError("This spare part is already added for order")
        return spare_part

    def save(self, commit=True):
        order_to_sp = super().save(commit=False)
        order_to_sp.order = self.order

        if commit:
            order_to_sp.save()
        return order_to_sp



class NewOrderForm(ModelForm):
    """Create blank order"""
    date_scheduled = forms.DateField(initial = datetime.date.today() + datetime.timedelta(days=1))

    def __init__(self, *args, **kvargs):
        self.client = kvargs.pop('client', None)
        super(ModelForm, self).__init__(*args, **kvargs)


    class Meta:
        model = Order
        fields = ("employee", "date_scheduled")

    def clean_date_scheduled(self):
        date = self.cleaned_data['date_scheduled']
        if date <= datetime.date.today():
            raise ValidationError("Date can't be in the past")
        if date > datetime.date.today() + datetime.timedelta(days=180):
            raise ValidationError("Date can't be later than 180 days after today")
        return date


    def save(self, commit=True):
        order = super().save(commit=False)
        order.approved = False
        order.client = self.client

        if commit:
            order.save()
        return order

