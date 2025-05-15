from django.contrib.auth import get_user_model
from django.shortcuts import render
from django.urls import reverse_lazy
from django.views.generic import CreateView
from django.contrib.auth.mixins import LoginRequiredMixin, PermissionRequiredMixin


from .models import Review
from .forms import ReviewNewForm


User = get_user_model()

class ReviewCreateView(CreateView, LoginRequiredMixin, PermissionRequiredMixin):
    permission_required = 'service_center.client_perm'
    form_class = ReviewNewForm
    template_name = 'reviews/review_new.html'
    success_url = reverse_lazy('reviews_list')

    def get_form_kwargs(self):
        kwargs = super(ReviewCreateView, self).get_form_kwargs()
        kwargs['user'] = self.request.user
        return kwargs



def review_list_view(req):
    reviews = Review.objects.order_by('-date')
    return render(req, 'reviews/reviews.html', {"reviews":reviews})

# Create your views here.
