# Generated by Django 2.1 on 2019-06-20 06:32

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('iplclone', '0010_auto_20190620_1141'),
    ]

    operations = [
        migrations.AlterField(
            model_name='match',
            name='player_of_match',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, to='iplclone.Player'),
        ),
    ]
