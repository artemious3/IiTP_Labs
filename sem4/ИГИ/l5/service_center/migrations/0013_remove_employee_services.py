# Generated by Django 5.1.9 on 2025-05-15 12:17

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('service_center', '0012_client_birth_date'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='employee',
            name='services',
        ),
    ]
