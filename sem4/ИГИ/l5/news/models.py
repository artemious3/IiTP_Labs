from django.conf import django
from django.db import models
from django.utils.timezone import now


class News(models.Model):
    head = models.CharField(max_length=255)
    content = models.TextField()
    date = models.DateField(default=now)

    def __str__(self):
        return self.head

# Create your models here.
