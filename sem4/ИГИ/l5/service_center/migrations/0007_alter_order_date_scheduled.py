# Generated by Django 5.1.9 on 2025-05-12 19:18

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('service_center', '0006_alter_order_employee_alter_order_spare_parts'),
    ]

    operations = [
        migrations.AlterField(
            model_name='order',
            name='date_scheduled',
            field=models.DateField(null=True),
        ),
    ]
