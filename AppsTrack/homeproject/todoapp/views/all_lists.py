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


class ToDoListView(LoginRequiredMixin, View):

    def get(self, request, urlname=None, **kwargs):
        urlname = resolve(request.path_info).url_name
        if urlname != 'all_lists' and urlname != 'edit_list' and urlname != 'add_list':
            raise Http404

        if urlname is None:
            urlname = resolve(request.path_info).url_name

        if urlname == 'all_lists':
            lists = ToDoList.objects.filter(user=User.objects.get(pk=request.user.id)).values('name', 'created', 'id')
            kwargs = {
                'records': lists,
                'fields': ['Name', 'Created'],
                'field_urls': ['all_items', None],
                'title': 'Todo Lists',
                'heading': 'ToDo Lists for user ' + request.user.username,
                'entity': 'ToDoList',
                'add_url': 'add_list',
                'edit_url': 'edit_list',
                'delete_url': 'delete_list',
                'permissions': request.user.get_all_permissions(),
                'child_template': get_template('records.html'),
            }

        else:
            if urlname == 'add_list':
                instance = None
            elif urlname == 'edit_list':
                instance = get_object_or_404(ToDoList, pk=kwargs['pk'])

            todo_list_form = ToDoListForm(instance=instance)
            kwargs = {
                'title': 'Add ToDo List | ToDo App',
                'heading': 'Add ToDo List',
                'forms': [todo_list_form],
                'child_template': get_template('add.html'),
            }


        return render(
            request,
            template_name='_base.html',
            context=kwargs
        )

    def post(self, request, **kwargs):
        urlname = resolve(request.path_info).url_name

        if urlname == 'edit_list':
            return self.put(request, **kwargs)

        if urlname != 'add_list':
            raise Http404


        todo_list_form = ToDoListForm(data=request.POST)

        if todo_list_form.is_valid():
            todo_list = todo_list_form.save(commit=False)
            todo_list.created = date()
            todo_list.user = request.user
            todo_list.save()

        kwargs['success'] = True
        messages.success(request, 'List created successfully')

        return self.get(request, urlname='all_lists', **kwargs)

    def put(self, request, **kwargs):
        if resolve(request.path_info).url_name != 'edit_list':
            raise Http404

        todo_list_form = ToDoListForm(
            data=request.POST,
            instance=get_object_or_404(ToDoList, **kwargs, user=request.user)
        )

        if todo_list_form.is_valid():
            todo_list_form.save()
        else:
            raise PermissionDenied

        kwargs['success'] = True
        messages.success(request, 'List updated successfully')

        return self.get(request, urlname='all_lists', **kwargs)

    def delete(self, request, **kwargs):
        if resolve(request.path_info).url_name != 'delete_list':
            raise Http404

        todo_list = get_object_or_404(ToDoList, **kwargs, user=request.user)
        todo_list.delete()

        kwargs['success'] = True
        messages.success(request, 'List deleted successfully')

        return self.get(request, urlname='all_lists', **kwargs)





