from rest_framework.serializers import *
from iplclone.models import *

class MatchAbstractSerializer(ModelSerializer):     # Only serializes and deserializes limited fields
    class Meta:
        model = Match
        fields = ('id', 'team1', 'team2', 'venue', 'toss_result', 'toss_decision', 'result', 'player_of_match')
