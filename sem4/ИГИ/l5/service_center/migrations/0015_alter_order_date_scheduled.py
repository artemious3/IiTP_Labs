# Generated by Django 5.1.9 on 2025-05-15 12:50

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('service_center', '0014_alter_order_employee'),
    ]

    operations = [
        migrations.AlterField(
            model_name='order',
            name='date_scheduled',
            field=models.DateField(default='0001-01-01'),
            preserve_default=False,
        ),
    ]
