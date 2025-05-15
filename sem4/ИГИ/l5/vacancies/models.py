from django.db import models

class Vacancy(models.Model):
    position = models.CharField(max_length=255)
    description = models.TextField()

    def __str__(self):
        return self.position


# Create your models here.
