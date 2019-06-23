from django.forms import *
from onlineapp.models import *


class AddCollegeForm(ModelForm):
    class Meta:
        model = College
        exclude = ['id']
        widgets = {
            'name': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'College Name'}),
            'location': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'College Location'}),
            'acronym': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'College Acronym'}),
            'contact': EmailInput(attrs={'class': 'input add-college-input', 'placeholder': 'College Email'})
        }


class StudentForm(ModelForm):
    class Meta:
        model = Student
        exclude = ['id', 'dob', 'college']
        widgets = {
            'name': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'Student Name'}),
            'email': EmailInput(attrs={'class': 'input add-college-input', 'placeholder': 'Student Email'}),
            'db_folder': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'Student database folder'}),
            'dropped out': CheckboxInput(attrs={'class': 'input add-college-input'}),
        }


class MockTest1Form(ModelForm):
    class Meta:
        model = MockTest1
        exclude = ['id', 'student', 'total']
        widgets = {
            'problem1': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'Problem 1'}),
            'problem2': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'Problem 2'}),
            'problem3': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'Problem 3'}),
            'problem4': TextInput(attrs={'class': 'input add-college-input', 'placeholder': 'Problem 4'}),
        }


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
