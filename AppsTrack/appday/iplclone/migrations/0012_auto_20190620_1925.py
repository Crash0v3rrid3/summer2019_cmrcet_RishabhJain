# Generated by Django 2.1 on 2019-06-20 13:55

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('iplclone', '0011_auto_20190620_1202'),
    ]

    operations = [
        migrations.AlterField(
            model_name='dismissal',
            name='fielder',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='fielder', to='iplclone.Player'),
        ),
    ]
