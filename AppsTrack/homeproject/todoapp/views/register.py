from django.views import *
from django.template.loader import *
from django.shortcuts import *
from todoapp.forms import *
from django.contrib import messages
from django.contrib.auth.mixins import *


class RegisterView(UserPassesTestMixin, View):

    def test_func(self):
        return not self.request.user.is_authenticated

    def get(self, request):
        register_form = RegisterForm()
        return render(
            request,
            template_name='_base.html',
            context={
                'title': 'Sign Up | ToDo App',
                'form': register_form,
                'child_template': get_template('signup.html')
            },
        )

    def post(self, request, **kwargs):
        registration_form = RegisterForm(data=request.POST)

        if registration_form.is_valid():
            try:
                user = User.objects.create_user(**registration_form.cleaned_data)

                user.save()

                messages.success(request, 'Registration successful')
                kwargs['success'] = True

                return redirect('login')
            except:
                messages.error(request, 'User already exists')
        else:
            messages.error(request, 'incorrect data')

        return render(
            request,
            template_name='_base.html',
            context={
                'child_template': get_template('signup.html'),
                'success': False,
                'form': registration_form,
            }
        )
