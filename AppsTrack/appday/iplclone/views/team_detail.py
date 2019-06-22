from django.views import *
from django.shortcuts import *
from iplclone.models import *
from django.db.models import *
from django.template.loader import *
from .utils import *


def calculatePosition(all_teams, team):
    itr, length = 0, len(all_teams)

    while itr < length:
        if all_teams[itr].id == team.id:
            return itr + 1
        itr += 1

    return -1

class TeamDetailView(View):
    def get(self, request, **kwargs):
        team = get_object_or_404(Team, **kwargs)
        seasons = Season.objects.all().order_by('year')
        positions = []

        for season in seasons:
            points = getPoints(season.year)
            position = calculatePosition(points, team)
            positions.append((season.year, position))

        return render(
            request,
            template_name='_base.html',
            context={
                'user': request.user,
                'title': 'IPL Points',
                'child_template': get_template('team_detail.html'),
                'team': team,
                'positions': positions
            }
        )
