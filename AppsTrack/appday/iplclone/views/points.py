from django.views import *
from django.shortcuts import *
from iplclone.models import *
from django.db.models import *
from django.template.loader import *
from .utils import *


class PointsView(View):
    def get(self, request, **kwargs):
        if request.GET.get('year') is not None:
            season_year = int(request.GET.get('year'))
        else:
            season_year = Season.objects.annotate(max=Max('year')).values('max')[0]['max']

        years = Season.objects.all().values_list('year')

        teams = getPoints(season_year)

        return render(
            request,
            template_name='_base.html',
            context={
                'user': request.user,
                'title': 'IPL Points',
                'child_template': get_template('points.html'),
                'years': sorted(years, key=lambda x: x[0]),
                'year': season_year,
                'teams': teams
            }
        )
