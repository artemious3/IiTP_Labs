from typing import Required
from django.contrib.auth import get_user_model
from django.contrib.auth.forms import UserCreationForm
from django import forms
from django.contrib.auth.models import Group
from service_center.models import Client

User = get_user_model()

class SignUpForm(UserCreationForm):
    # Additional fields to User itself
    first_name = forms.CharField(max_length=30)
    last_name = forms.CharField(max_length=30)
    email = forms.EmailField(required=True)

    # Fields for Client
    phone_number = forms.CharField(max_length=16)
    passport_id = forms.CharField(max_length=10)
    address = forms.CharField(max_length=64)
    birth_date = forms.DateField()


    def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)
            self.fields['first_name'].label = 'First Name'
            self.fields['last_name'].label = 'Last Name'

    class Meta:
        model = User
        fields = ('username', 'first_name', 'last_name', 'email', 'password1', 'password2' )
        help_texts = {
            'username': None,
        }

    def save(self, commit=True):
        user = super().save()

        client_group = Group.objects.get(name='Client')
        user.groups.add(client_group)

        Client.objects.create(
                user=user,
                phone_number = self.cleaned_data['phone_number'],
                passport_id = self.cleaned_data['passport_id'],
                address = self.cleaned_data['address'],
                birth_date = self.cleaned_data['birth_date']
            )

        return user




