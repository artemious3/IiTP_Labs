from datetime import timezone
from django.contrib.auth import get_user_model
from django.utils.timezone import now
from django.db import models
from django.core.validators import MaxValueValidator, MinValueValidator

User = get_user_model()

class Review(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    date = models.DateField(default=now)
    mark = models.IntegerField(validators=[MaxValueValidator(5), MinValueValidator(1)])
    text = models.TextField(blank=False)

# Create your models here.
