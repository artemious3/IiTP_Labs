from django import urls
from django.contrib import admin
from django.urls import path, include
from . import views

urlpatterns = [
        path('list', views.review_list_view, name='reviews_list'),
        path('new', views.ReviewCreateView.as_view(), name='review_create')
]

