from django.views import *
from django.shortcuts import *
from iplclone.models import *
from django.db.models import *
from django.template.loader import *
from .utils import *

class HomepageView(View):
    def get(self, request, **kwargs):
        pass
