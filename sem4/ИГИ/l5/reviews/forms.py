from django.forms import ModelForm, ValidationError

from news import models
from .models import Review


class ReviewNewForm(ModelForm):
    def __init__(self, *args, **kvargs):
        self.user = kvargs.pop('user', None)
        super(ModelForm, self).__init__(*args, **kvargs)

    class Meta:
        model = Review
        fields = ("mark", "text")

    def save(self, commit=True):
        review = super().save(commit=False)
        review.user = self.user

        if commit:
            review.save()
        return review
