from django.shortcuts import render
from django.views.generic import ListView
from .models import Vacancy
from django.shortcuts import get_object_or_404, render, render

class NewsListView(ListView):
    model = Vacancy
    paginate_by = 10


def vacancy_detail_view(req, vac_id):
    vacancy = get_object_or_404(Vacancy, pk=vac_id)
    return render(req, "vacancies/vacancy_detail.html", {"vacancy":vacancy})
# Create your views here.
