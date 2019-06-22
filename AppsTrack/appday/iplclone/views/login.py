from django.views import *
from django.template.loader import *
from django.shortcuts import *
from django.contrib.auth import *
from django.contrib import messages
from django.contrib.auth.mixins import *
from iplclone.forms import *


def logout_user(request):
    if request.user.is_authenticated:
        logout(request)
    return redirect('login')


class LoginView(UserPassesTestMixin, View):

    def test_func(self):
        return not self.request.user.is_authenticated

    def get(self, request, **kwargs):
        if not request.user.is_authenticated:
            login_form = LoginForm()
            return render(
                request,
                template_name='_base.html',
                context={
                    **request.GET,
                    **kwargs,
                    'title': 'Login | IPL Clone',
                    'form': login_form,
                    'child_template': get_template('login.html'),
                    'user': request.user
                },
            )
        else:
            return redirect('homepage')

    def post(self, request, **kwargs):
        if not request.user.is_authenticated:
            #form = LoginForm(data=request.POST)
            user = authenticate(
                request,
                username=(request.POST['username']),
                password=(request.POST['password']),
            )

            if user is None:
                kwargs['success'] = False
                messages.error(request, 'Invalid Credentials')
                return self.get(request, **kwargs)
            else:
                login(request, user)
                return redirect('homepage')

        return redirect('homepage')
