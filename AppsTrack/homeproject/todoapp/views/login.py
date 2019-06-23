from django.views import *
from django.template.loader import *
from django.shortcuts import *
from todoapp.forms import *
from django.contrib.auth import *
from django.contrib import messages
from django.contrib.auth.mixins import *
from django.core.exceptions import *


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
                    'title': 'Login | ToDo App',
                    'form': login_form,
                    'child_template': get_template('login.html'),
                    'user': request.user
                },
            )
        else:
            return redirect('all_lists')

    def post(self, request, **kwargs):
        if not request.user.is_authenticated:
            form = LoginForm(data=request.POST)

            if not form.is_valid():
                raise PermissionDenied

            user = authenticate(
                request,
                username=form.cleaned_data['username'],
                password=form.cleaned_data['password'],
            )

            if user is None:
                kwargs['success'] = False
                messages.error(request, 'Invalid Credentials')
                return self.get(request, **kwargs)
            else:
                login(request, user)
                return redirect('all_lists')

        return redirect('all_lists')
