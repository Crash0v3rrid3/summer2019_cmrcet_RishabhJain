"""classproject URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
import classproject.settings as settings
from django.urls import include, path
import debug_toolbar
import onlineapp.views as views
from django.shortcuts import *

urlpatterns = [
    path('', lambda req: redirect('login')),
    path('admin/', admin.site.urls),
    path('login/', views.LoginView.as_view(), name='login'),
    path('signup/', views.RegisterView.as_view(), name='signup'),
    path('logout/', views.logout_user, name='logout'),
    #path(r'^api-auth/', include('rest_framework.urls')),
    path('', include('onlineapp.urls'), name='onlineapp'),
]

if settings.DEBUG:
    urlpatterns += [path('__debug__/', include(debug_toolbar.urls))]
