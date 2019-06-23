from django.forms import *
from todoapp.models import *
from django import forms


class LoginForm(Form):
    username = CharField(
        label='Username',
        max_length=75,
        widget=TextInput(
            attrs={
                'class': 'input',
                'placeholder': 'Username'
            }
        )
    )
    password = CharField(
        label='Password',
        max_length=75,
        widget=PasswordInput(
            attrs={
                'class': 'input',
                'placeholder': 'Password'
            }
        )
    )

class ToDoListForm(ModelForm):
    class Meta:
        model = ToDoList
        fields = ['name']
        widgets = {
            'name': TextInput(attrs={'class': 'input', 'placeholder': 'Name'})
        }


class ToDoItemForm(ModelForm):
    class Meta:
        model = ToDoItem
        exclude = ['id', 'list']
        widgets = {
            'description': Textarea(attrs={'class': 'textarea is-primary', 'placeholder': 'Look into django debug toolbar...'}),
            'due_date': forms.DateTimeInput(attrs={}),
            'completed': RadioSelect(attrs={'class': 'radio'}),
        }


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
