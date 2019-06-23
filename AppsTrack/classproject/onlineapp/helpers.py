from django.shortcuts import *
from django.views import *
from onlineapp.models import *
from django.template.loader import *
from django.shortcuts import *
from django.urls import *
from onlineapp.forms import *
from django.contrib.auth import *

def is_authenticated(request):
    if request.user.is_authenticated:
        return None
    return redirect('login')