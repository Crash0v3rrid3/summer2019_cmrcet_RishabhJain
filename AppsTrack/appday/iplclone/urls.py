from django.urls import path
from . import views


urlpatterns = [
    path('', views.SeasonAbstractView.as_view(), name='homepage'),
    path('seasons/', views.SeasonAbstractView.as_view(), name='seasons'),
    path('points/', views.PointsView.as_view(), name='points'),
    path('match_detail/<int:pk>', views.MatchDetailView.as_view(), name='match_detail'),
    path('team_detail/<int:pk>', views.TeamDetailView.as_view(), name='team_detail'),
    path('login/', views.LoginView.as_view(), name='login'),
    path('register/', views.RegisterView.as_view(), name='register'),
    path('logout/', views.logout_user, name='logout'),
]