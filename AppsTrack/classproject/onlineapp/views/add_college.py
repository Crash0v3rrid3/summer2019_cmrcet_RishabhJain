from onlineapp.helpers import *
from django.contrib.auth.mixins import *


class AddCollegeView(PermissionRequiredMixin, LoginRequiredMixin, View):
    permission_required = ('onlineapp.add_college', 'onlineapp.change_college', 'onlineapp.delete_college')

    def servePage(self, request, **kwargs):
        kwargs['child_template'] = get_template('add.html')
        return render(
            request,
            template_name='_base.html',
            context=kwargs
        )

    def get(self, request, **kwargs):
        college = None
        if resolve(request.path_info).url_name == 'edit_college':
            college = get_object_or_404(College, pk=kwargs['pk'])
            kwargs['title'] = 'Edit College | Online App'
            kwargs['heading'] = 'Edit College'

        form = AddCollegeForm(instance=college)

        if kwargs.get('title') is None:
            kwargs['title'] = 'Add College | Online App'
            kwargs['heading'] = 'Add College'

        kwargs = {
            'forms': [form],
            **kwargs,
            'permissions': request.user.get_all_permissions(),
        }

        return self.servePage(request, **kwargs)

    def post(self, request, **kwargs):
        urlname = resolve(request.path_info).url_name
        college = None

        if urlname == 'delete_college':     # if Delete college
            college = get_object_or_404(College, **kwargs)
            college.delete()
            return redirect('all_colleges')

        elif urlname == 'edit_college':     # if edit college
            college = get_object_or_404(College, **kwargs)
            kwargs['title'] = 'Edit College | Online App'
            kwargs['heading'] = 'Edit College'

                                            # else add college
        form = AddCollegeForm(
            data=request.POST,
            instance=college,
        )

        success = False

        if form.is_valid():
            form.save()
            success = True

        kwargs = {
            **kwargs,
            'success': success,
            'permissions': request.user.get_all_permissions(),
        }

        return self.get(request, **kwargs)
