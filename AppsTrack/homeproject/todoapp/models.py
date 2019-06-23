from django.db import models
from datetime import *
from django.contrib.auth.models import User


class ToDoList(models.Model):
    name = models.CharField(max_length=255)
    created = models.DateTimeField(default=datetime.now)
    user = models.ForeignKey(User, on_delete=models.CASCADE)

    def __str__(self):
        return self.name


class ToDoItem(models.Model):
    description = models.CharField(max_length=255)
    due_date = models.DateTimeField(blank=True, null=True)
    completed = models.BooleanField()
    list = models.ForeignKey(ToDoList, on_delete=models.CASCADE)

    def __str__(self):
        return self.description[10] + '...'
