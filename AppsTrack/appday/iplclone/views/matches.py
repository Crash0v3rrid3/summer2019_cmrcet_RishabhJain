from django.views import *
from django.shortcuts import *
from django.template.loader import *
from django.db.models import *
from iplclone.template_filters import *             # Important
from .utils import *


class MatchDetailView(View):

    def get(self, request, **kwargs):
        if not request.user.is_authenticated:
            return redirect('login')

        match = Match.objects.get(**kwargs)

        team1_total_runs = Runs.objects.filter(
                    ball__over__ining__match=match,
                    ball__over__ining__batting_team=match.team1
                )\
            .annotate(total_score=Sum('total_runs'))\
            .annotate(extra_score=Sum('extra_runs'))\
            .values_list('total_score', 'extra_score')

        team1_total_runs = (sum(map(lambda x: x[0], team1_total_runs)),
                            sum(map(lambda x: x[1], team1_total_runs)))

        team2_total_runs = Runs.objects.filter(
                    ball__over__ining__match=match,
                    ball__over__ining__batting_team=match.team2
                )\
            .annotate(total_score=Sum('total_runs'))\
            .annotate(extra_score=Sum('extra_runs'))\
            .values_list('total_score', 'extra_score')

        team2_total_runs = (sum(map(lambda x: x[0], team2_total_runs)),
                            sum(map(lambda x: x[1], team2_total_runs)))

        team1_top_run_scorer = Runs.objects\
            .filter(ball__over__ining__match=match, ball__over__ining__batting_team=match.team1.id)\
            .values('ball__batsman')\
            .annotate(total_score=Sum('total_runs'))\
            .order_by('-total_score')[: 3]

        team2_top_run_scorer = Runs.objects \
            .filter(ball__over__ining__match=match, ball__over__ining__batting_team=match.team2) \
            .values('ball__batsman') \
            .annotate(total_score=Sum('total_runs')) \
            .order_by('-total_score')[:3]

        team1_overs = getOvers(match, 1)
        team2_overs = getOvers(match, 2)

        for scorer in team1_top_run_scorer:
            scorer['batsman'] = Player.objects.get(pk=scorer['ball__batsman'])
        for scorer in team2_top_run_scorer:
            scorer['batsman'] = Player.objects.get(pk=scorer['ball__batsman'])


        team1_top_wicket_taker = Dismissal.objects\
            .filter(ball__over__ining__batting_team=match.team2)\
            .values('ball__bowler')\
            .annotate(total_wickets=Count('id'))\
            .order_by('-total_wickets')[:3]

        team2_top_wicket_taker = Dismissal.objects\
             .filter(ball__over__ining__batting_team=match.team1) \
             .values('ball__bowler')\
             .annotate(total_wickets=Count('id'))\
             .order_by('-total_wickets')[:3]

        for bowler in team1_top_wicket_taker:
            bowler['bowler'] = Player.objects.get(pk=bowler['ball__bowler'])
        for bowler in team2_top_wicket_taker:
            bowler['bowler'] = Player.objects.get(pk=bowler['ball__bowler'])


        return render(
            request,
            template_name='_base.html',
            context={
                'team1_total_runs': team1_total_runs,
                'team2_total_runs': team2_total_runs,
                'team1_top_run_scorers': team1_top_run_scorer,
                'team2_top_run_scorers': team2_top_run_scorer,
                'team1_top_wicket_takers': team1_top_wicket_taker,
                'team2_top_wicket_takers': team2_top_wicket_taker,
                'match': match,
                'team1_overs': team1_overs,
                'team2_overs': team2_overs,
                'user': request.user,
                'title': 'IPL Seasons',
                'child_template': get_template('match_detail.html'),
            }
        )