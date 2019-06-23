from onlineapp.helpers import *
from django.contrib.auth.mixins import *
from rest_framework.decorators import *
from rest_framework.views import *
from onlineapp.serializers import *
from rest_framework.authentication import *
from rest_framework.permissions import IsAuthenticated
import os


@api_view(['POST', 'GET', 'PUT', 'DELETE'])
#@authentication_classes((TokenAuthentication, ))
# @authentication_classes((BasicAuthentication, ))
# @permission_classes((IsAuthenticated, ))
def rest_colleges(request, **kwargs):
    if request.method == 'POST':
        college = CollegeSerializer(data=request.data)

        if college.is_valid():
            college.save()
        else:
            return HttpResponse("", status=400)
    elif request.method == 'PUT':
        college = get_object_or_404(College, pk=request.data.get('id'))
        college = CollegeSerializer(data=request.data, instance=college)
        college.save()
    elif request.method == 'DELETE':
        college = get_object_or_404(College, pk=request.data.get('id'))
        college.delete()
    if kwargs:
        college = get_object_or_404(College, **kwargs)
        many = False
    else:
        college = College.objects.all()
        many = True
    serialized_obj = CollegeSerializer(college, many=many)
    return Response(serialized_obj.data)


class CollegeView(PermissionRequiredMixin, LoginRequiredMixin, View):
    permission_required = ('onlineapp.view_college', 'onlineapp.view_student')

    def servePage(self, request, **kwargs):
        kwargs['child_template'] = get_template('records.html')
        return render(
            request,
            template_name='_base.html',
            context=kwargs,
        )

    def get(self, request, *args, **kwargs):
        url_name = resolve(request.path_info).url_name

        if url_name == 'college_with_id' or url_name == 'college_with_acronym':
            college = College.objects.filter(pk=kwargs['college_id']).values_list('name')
            students = Student.objects.filter(
                **kwargs,
                dropped_out=False,
            ).values(
                'name',
                'email',
                'mocktest1__total',
                'id',
            )

            try:
                kwargs = {
                    **kwargs,
                    'records': students,
                    'fields': ['Name', 'Email', 'Total Marks'],
                    'field_urls': [None, None, None],
                    'title': 'Students | ' + college[0][0],
                    'heading': 'Students in ' + college[0][0],
                    'entity': 'Student',
                    'add_url': 'add_student',
                    'edit_url': 'edit_student',
                    'delete_url': 'delete_student',
                    'id': kwargs['college_id'],
                    'permissions': request.user.get_all_permissions(),
                }
            except:
                raise Http404('No college found')
        else:
            colleges = College.objects.values('name', 'acronym', 'id')
            kwargs = {
                **kwargs,
                'fields': ['name', 'acronym'],
                'field_urls': [None, 'college_with_id'],
                'records': colleges,
                'title': 'Colleges | MissionRnD',
                'heading': 'Participant Colleges',
                'entity': 'College',
                'add_url': 'add_college',
                'edit_url': 'edit_college',
                'delete_url': 'delete_college',
                'permissions': request.user.get_all_permissions(),
            }

        return self.servePage(request, **kwargs)
