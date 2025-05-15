from django.shortcuts import render
from .models import FAQ
from django.shortcuts import render

def view(req):
    faqs = FAQ.objects.all();
    return render(req, 'faq/faq.html', {"faq_list":faqs})

# Create your views here.
