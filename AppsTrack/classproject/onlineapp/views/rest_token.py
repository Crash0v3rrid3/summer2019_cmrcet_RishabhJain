from rest_framework.decorators import *
from rest_framework.views import *
from rest_framework.authtoken.models import Token
from rest_framework.authtoken.serializers import AuthTokenSerializer


@api_view(['POST'])
def generate_token(request, **kwargs):
    serialized_data = AuthTokenSerializer(data=request.data)
    serialized_data.is_valid(raise_exception=True)

    user = serialized_data.validated_data['user']
    token, created = Token.objects.get_or_create(user=user)

    return Response({
        'token': token.key,
        'user_id': user.pk,
    })
