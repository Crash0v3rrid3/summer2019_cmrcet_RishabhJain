# Generated by Django 2.2.1 on 2019-06-17 16:07

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Ball',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
            ],
        ),
        migrations.CreateModel(
            name='City',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=60, unique=True)),
            ],
        ),
        migrations.CreateModel(
            name='DismissalKind',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=50)),
            ],
        ),
        migrations.CreateModel(
            name='Ining',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('ining_number', models.BooleanField()),
                ('batting_team', models.BooleanField()),
            ],
        ),
        migrations.CreateModel(
            name='Player',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=50)),
            ],
        ),
        migrations.CreateModel(
            name='Result',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('result', models.CharField(max_length=30)),
            ],
        ),
        migrations.CreateModel(
            name='Stadium',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=60, unique=True)),
            ],
        ),
        migrations.CreateModel(
            name='Team',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=60, unique=True)),
            ],
        ),
        migrations.CreateModel(
            name='Umpire',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=50)),
            ],
        ),
        migrations.CreateModel(
            name='Venue',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('city', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.City')),
                ('stadium', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Stadium')),
            ],
        ),
        migrations.CreateModel(
            name='Season',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('year', models.IntegerField()),
                ('winner', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Team')),
            ],
        ),
        migrations.CreateModel(
            name='Runs',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('wide_runs', models.IntegerField()),
                ('bye_runs', models.IntegerField()),
                ('leg_bye_runs', models.IntegerField()),
                ('no_ball_runs', models.IntegerField()),
                ('penalty_runs', models.IntegerField()),
                ('batsman_runs', models.IntegerField()),
                ('extra_runs', models.IntegerField()),
                ('total_runs', models.IntegerField()),
                ('ball', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Ball')),
            ],
        ),
        migrations.CreateModel(
            name='Over',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('is_super_over', models.BooleanField()),
                ('ining', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Ining')),
            ],
        ),
        migrations.CreateModel(
            name='Match',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('winner', models.BooleanField()),
                ('date', models.DateTimeField()),
                ('toss_winner', models.BooleanField()),
                ('toss_decision', models.BooleanField()),
                ('win_by_runs', models.IntegerField(null=True)),
                ('win_by_wickets', models.IntegerField(null=True)),
                ('dl_applied', models.BooleanField()),
                ('player_of_match', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Player')),
                ('result', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Result')),
                ('season', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Season')),
                ('team1', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='team1', to='iplclone.Team')),
                ('team2', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='team2', to='iplclone.Team')),
                ('umpire1', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='umpire1', to='iplclone.Umpire')),
                ('umpire2', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='umpire2', to='iplclone.Umpire')),
                ('umpire3', models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='umpire3', to='iplclone.Umpire')),
                ('venue', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Venue')),
            ],
        ),
        migrations.AddField(
            model_name='ining',
            name='match',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Match'),
        ),
        migrations.CreateModel(
            name='Dismissal',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('ball', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Ball')),
                ('dismissal_kind', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.DismissalKind')),
                ('fielder', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='fielder', to='iplclone.Player')),
                ('player_dismissed', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='player_dismissed', to='iplclone.Player')),
            ],
        ),
        migrations.AddField(
            model_name='ball',
            name='batsman',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='batsman', to='iplclone.Player'),
        ),
        migrations.AddField(
            model_name='ball',
            name='bowler',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='bowler', to='iplclone.Player'),
        ),
        migrations.AddField(
            model_name='ball',
            name='non_striker',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='non_striker', to='iplclone.Player'),
        ),
        migrations.AddField(
            model_name='ball',
            name='over',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='iplclone.Over'),
        ),
    ]
