import openpyxl
import django
import os
import datetime

# setup django
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'appday.settings')
django.setup()
from iplclone.models import *

deliveries = openpyxl.load_workbook('deliveries.xlsx')

deliveries_sheet = deliveries.active

id = None
inings = set()
rows = [[y.value for y in x] for x in deliveries_sheet.rows]
rows = rows[1: ]
pk = 1

for row in rows:
    rowTuple = row

    if rowTuple[19]:
        ball = Ball.objects.get(
            pk=pk
        )

        dismissal = Dismissal(
            ball=ball,
            player_dismissed=Player.objects.get(name=rowTuple[18]),
            dismissal_kind=DismissalKind.objects.get(name=rowTuple[19]),
            fielder=Player.objects.get(name=rowTuple[20]) if rowTuple[20] else None
        )

        dismissal.save()
    pk += 1


