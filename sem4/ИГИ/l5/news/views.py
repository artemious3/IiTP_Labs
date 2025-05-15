# from django.shortcuts import render
from django.views.generic import ListView
from .models import News
from django.shortcuts import get_object_or_404, render, render

class NewsListView(ListView):
    model = News
    paginate_by = 10


def news_detail_view(req, news_id):
    news = get_object_or_404(News, pk=news_id)
    return render(req, "news/news_detail.html", {"news":news})
