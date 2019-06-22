from django.forms import *


class LoginForm(Form):
    username = CharField(label='Username', max_length=75, widget=TextInput(attrs={'class': 'input', 'placeholder': 'Username'}))
    password = CharField(label='Password', max_length=75, widget=PasswordInput(attrs={'class': 'input', 'placeholder': 'Password'}))


class RegisterForm(Form):
    first_name = CharField(label='First Name', max_length=75,
                         widget=TextInput(attrs={'class': 'input', 'placeholder': 'First Name'}))
    last_name = CharField(label='Last Name', max_length=75,
                         widget=TextInput(attrs={'class': 'input', 'placeholder': 'Last Name'}))
    email = CharField(label='Email', max_length=75,
                          widget=EmailInput(attrs={'class': 'input', 'placeholder': 'Email'}))
    username = CharField(label='Username', max_length=75,
                         widget=TextInput(attrs={'class': 'input', 'placeholder': 'Username'}))
    password = CharField(label='Password', max_length=75,
                         widget=PasswordInput(attrs={'class': 'input', 'placeholder': 'Password'}))
