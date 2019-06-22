from django.db import models


class Team(models.Model):
    #maybe sponsoror, owner, blah blah
    name = models.CharField(max_length=60, unique=True)
    logo = models.CharField(max_length=150, null=True, blank=True)

    def __str__(self):
        return self.name


class Season(models.Model):
    # maybe name?
    year = models.IntegerField()


class City(models.Model):
    name = models.CharField(max_length=60, unique=True)

    def __str__(self):
        return self.name


class Stadium(models.Model):
    # maybe radius, field type blah
    name = models.CharField(max_length=60, unique=True)

    def __str__(self):
        return self.name


class Venue(models.Model):
    # A city can have multiple stadiums
    stadium = models.ForeignKey(Stadium, on_delete=models.CASCADE)
    city = models.ForeignKey(City, on_delete=models.CASCADE)


class Result(models.Model):
    # normal, tie, not sure...
    result = models.CharField(max_length=30)
    points = models.IntegerField()


class Player(models.Model):
    # A player will have much more
    name = models.CharField(max_length=50)

    def __str__(self):
        return self.name


class Umpire(models.Model):
    name = models.CharField(max_length=50)

    def __str__(self):
        return self.name


class Match(models.Model):
    team1 = models.ForeignKey(Team, on_delete=models.CASCADE, related_name='team1')
    team2 = models.ForeignKey(Team, on_delete=models.CASCADE, related_name='team2')
    season = models.ForeignKey(Season, on_delete=models.CASCADE)
    winner = models.BooleanField()
    venue = models.ForeignKey(Venue, on_delete=models.CASCADE)
    date = models.DateTimeField()
    toss_winner = models.BooleanField()
    toss_decision = models.BooleanField()
    result = models.ForeignKey(Result, on_delete=models.CASCADE)
    win_by_runs = models.IntegerField(null=True)
    win_by_wickets = models.IntegerField(null=True)
    dl_applied = models.BooleanField()
    player_of_match = models.ForeignKey(Player, on_delete=models.CASCADE, null=True, blank=True)
    umpire1 = models.ForeignKey(Umpire, on_delete=models.CASCADE, related_name='umpire1', null=True, blank=True)
    umpire2 = models.ForeignKey(Umpire, on_delete=models.CASCADE, related_name='umpire2', null=True, blank=True)
    umpire3 = models.ForeignKey(Umpire, on_delete=models.CASCADE, null=True, blank=True, related_name='umpire3')
    tmp_match_id = models.IntegerField()


class Ining(models.Model):
    match = models.ForeignKey(Match, on_delete=models.CASCADE)
    inings = models.IntegerField()
    batting_team = models.ForeignKey(Team, on_delete=models.CASCADE, related_name='batting_team')


class Over(models.Model):
    ining = models.ForeignKey(Ining, on_delete=models.CASCADE)
    is_super_over = models.BooleanField()


class Ball(models.Model):
    over = models.ForeignKey(Over, on_delete=models.CASCADE)
    batsman = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='batsman')
    non_striker = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='non_striker')
    bowler = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='bowler')        # Bowler can change in the middle of the over


class DismissalKind(models.Model):
    name = models.CharField(max_length=50)


class Dismissal(models.Model):
    ball = models.ForeignKey(Ball, on_delete=models.CASCADE)
    player_dismissed = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='player_dismissed')
    dismissal_kind = models.ForeignKey(DismissalKind, on_delete=models.CASCADE)
    fielder = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='fielder', null=True, blank=True)


class Runs(models.Model):
    ball = models.OneToOneField(Ball, on_delete=models.CASCADE, related_name='ball')
    wide_runs = models.IntegerField()
    bye_runs = models.IntegerField()
    leg_bye_runs = models.IntegerField()
    no_ball_runs = models.IntegerField()
    penalty_runs = models.IntegerField()
    batsman_runs = models.IntegerField()
    extra_runs = models.IntegerField()
    total_runs = models.IntegerField()
