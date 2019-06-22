from django.views import *
from django.shortcuts import *
from iplclone.models import *
from django.db.models import *
from django.template.loader import *


class SeasonAbstractView(View):
    def get(self, request, **kwargs):
        if request.GET.get('year') is not None:
            season_year = int(request.GET.get('year'))
        else:
            season_year = Season.objects.annotate(max=Max('year')).values('max')[0]['max']

        if request.GET.get('start') and request.GET.get('end'):
            offset = int(request.GET.get('start') or 1)
            limit = int(request.GET.get('end') or 8) - offset

            matches = Match.objects.filter(
                season__year=season_year
            )[offset: offset + limit]
        else:
            matches = Match.objects.filter(
                season__year=season_year
            )

        years = Season.objects.all().values_list('year')

        return render(
            request,
            template_name='_base.html',
            context={
                'matches': matches,
                'user': request.user,
                'title': 'IPL Seasons',
                'child_template': get_template('season_abstract.html'),
                'years': sorted(years, key=lambda x: x[0]),
                'year': season_year
            }
        )

    def post(self, request, **kwargs):
        pass
