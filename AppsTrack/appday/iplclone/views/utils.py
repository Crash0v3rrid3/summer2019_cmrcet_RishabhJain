from iplclone.models import *
from django.db.models import *

def getPoints(season_year):
    teams = Team.objects.all()

    for team in teams:
        matches_won = Match.objects.filter(team1=team, winner=0, season__year=season_year) \
                      | Match.objects.filter(team1=team, winner=1, season__year=season_year)
        points = matches_won.values('result__points').annotate(points=Sum('result__points'))
        total_points = None
        try:
            total_points = points[0]['points']
            total_points += points[1]['points']
        except Exception as err:
            if total_points is None:
                total_points = 0

        setattr(team, 'points', total_points)

    teams = sorted(teams, key=lambda team: team.points, reverse=True)

    return teams


def _assign_x(dictionary, id, x):
    try:
        dictionary[id] = x
    except:
        setattr(dictionary, id, x)


def getOvers(match, inings=None):

    # Fetch balls
    balls = Ball.objects.filter(over__ining__match=match, over__ining__inings=inings)

    # Map balls using their id's
    balls_index = {}
    for ball in balls: _assign_x(balls_index, ball.id, ball)

    # Fetch runs
    runs = Runs.objects.filter(ball__over__ining__match=match, ball__over__ining__inings=inings)
    # map runs to their balls
    for run in runs: _assign_x(balls_index[run.ball.id], 'runs', run)

    # fetch dismissals
    dismissals = Dismissal.objects.filter(
        ball__over__ining__match=match,
        ball__over__ining__inings=inings
    )

    # Map dismissal to their their ball
    for dismissal in dismissals: _assign_x(balls_index[dismissal.ball.id], 'dismissal', dismissal)

    overs = {}

    # Add ball to overs according to id's
    for ball in balls_index.values():
        if overs.get(ball.over.id):
            overs[ball.over.id].append(ball)
        else:
            overs[ball.over.id] = [ball]

    # Sort overs according to id
    overs = tuple(map(lambda x: x[1], sorted(overs.items(), key=lambda x: x[0])))

    return overs
