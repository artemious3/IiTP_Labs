from django.urls import path
from . import views


urlpatterns = [
        path('', views.NewsListView.as_view(), name='list'),
        path('<int:news_id>/', views.news_detail_view, name='news_detail')
    ]
