from django.db import models
from django.utils.timezone import now

class FAQ(models.Model):
    question = models.CharField(max_length=255)
    answer = models.TextField()
    date = models.DateField(default=now)

