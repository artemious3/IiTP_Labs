
from . import views
from django.urls import include,path

urlpatterns = [
        path('', views.NewsListView.as_view(), name='list'),
        path('<int:vac_id>/', views.vacancy_detail_view, name='vacancy_detail')
    ]
