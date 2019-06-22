import openpyxl
import django
import os

# setup django
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'appday.settings')
django.setup()
from iplclone.models import *

deliveries = openpyxl.load_workbook('deliveries.xlsx')

deliveries_sheet = deliveries.active

rows = [[y.value for y in x] for x in deliveries_sheet.rows]
rows = rows[1: ]
pk = 1
runs_s = []

for row in rows:

    runs = Runs(
        ball_id=pk,
        wide_runs=row[10],
        bye_runs=row[11],
        leg_bye_runs=row[12],
        no_ball_runs=row[13],
        penalty_runs=row[14],
        batsman_runs=row[15],
        extra_runs=row[16],
        total_runs=row[17],
    )

    runs.save()
    pk += 1
