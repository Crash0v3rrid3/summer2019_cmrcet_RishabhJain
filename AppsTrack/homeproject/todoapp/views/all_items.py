from django.views import *
from django.template.loader import *
from django.shortcuts import *
from todoapp.forms import *
from django.contrib import messages
from django.contrib.auth.mixins import *
from django.core.exceptions import *
from todoapp.models import *
from datetime import *
from django.urls import *


class ToDoItemView(LoginRequiredMixin, View):

    def servePage(self, request, **kwargs):
        kwargs['child_template'] = get_template('records.html')



    def get(self, request, urlname=None, **kwargs):
        if urlname is None:
            urlname = resolve(request.path_info).url_name
        if urlname != 'all_items' and urlname != 'edit_item' and urlname != 'add_item':
            raise Http404

        if urlname == 'all_items':
            list = ToDoList.objects.get(user=User.objects.get(pk=request.user.id), pk=kwargs.get('pk'))
            items = ToDoItem.objects.filter(list=list).values('description', 'due_date', 'completed', 'id')

            kwargs = {
                'records': items,
                'fields': ['Description', 'Due Date', 'Completed'],
                'field_urls': [None, None, None],
                'title': 'Todo Items',
                'heading': 'ToDo Items for list ' + list.name,
                'entity': 'ToDoItem',
                'add_url': 'add_item',
                'edit_url': 'edit_item',
                'delete_url': 'delete_item',
                'id': list.id,
                'permissions': request.user.get_all_permissions(),
                'child_template': get_template('records.html'),
            }

        else:
            if urlname == 'add_item':
                instance = None
            elif urlname == 'edit_item':
                instance = get_object_or_404(ToDoItem, pk=kwargs['pk'], list=ToDoList.objects.get(pk=kwargs.get('list_id')))

            todo_item_form = ToDoItemForm(instance=instance)
            kwargs = {
                'title': 'Add ToDo Item | ToDo App',
                'heading': 'Add ToDo Item',
                'forms': [todo_item_form],
                'child_template': get_template('add.html'),
            }

        return render(
            request,
            template_name='_base.html',
            context=kwargs
        )


    def post(self, request, **kwargs):
        if resolve(request.path_info).url_name != 'add_item':
            raise Http404

        todo_item_form = ToDoItemForm(data=request.POST)

        if todo_item_form.is_valid():
            todo_item = todo_item_form.save(commit=False)
            todo_item.list = get_object_or_404(ToDoList, pk=kwargs.get('list_id'), user=request.user)
            todo_item.save()

        kwargs['success'] = True
        messages.success(request, 'Item created successfully')

        return self.get(request, **kwargs)

    def put(self, request, **kwargs):
        if resolve(request.path_info).url_name != 'edit_item':
            raise Http404

        todo_item_form = ToDoItemForm(
            data=request.POST,
            instance=get_object_or_404(ToDoItem, **kwargs, list__user=request.user)
        )

        if todo_item_form.is_valid():
            todo_item_form.save()
        else:
            raise PermissionDenied

        kwargs['success'] = True
        messages.success(request, 'Item updated successfully')

        return self.get(request, **kwargs)

    def delete(self, request, **kwargs):
        if resolve(request.path_info).url_name != 'delete_item':
            raise Http404

        todo_item = get_object_or_404(ToDoItem, **kwargs, list__user=request.user)
        todo_item.delete()

        kwargs['success'] = True
        messages.success(request, 'Item deleted successfully')

        return self.get(request, **kwargs)
