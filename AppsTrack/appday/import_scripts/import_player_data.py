import openpyxl
import django
import os
import datetime

#setup django
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'appday.settings')
django.setup()
from iplclone.models import *

# open sheet
# matches = openpyxl.load_workbook('matches.xlsx')
deliveries = openpyxl.load_workbook('deliveries.xlsx')
#
# matches_sheet = matches.active
deliveries_sheet = deliveries.active

# # rows = [[x[6].value, x[7].value, x[8].value] for x in deliveries_sheet.rows]
# players = set()
# players_objs = []

rows = [[y.value for y in x] for x in deliveries_sheet.rows]
rows = rows[1: ]
players = set()

for row in rows:
    rowTuple = row

    if rowTuple[20]:
        players.add(rowTuple[20])


for player in players:
    try:
        print(Player.objects.get(name=player))
    except:
        Player(
            name=player
        ).save()

