from onlineapp.helpers import *
from rest_framework.views import *
from onlineapp.serializers import *
from rest_framework.response import *
from rest_framework.authentication import *
from rest_framework.permissions import IsAuthenticated


class ListStudent(APIView):
    authentication_classes = (TokenAuthentication, )
    permission_classes = (IsAuthenticated, )

    def get(self, request, **kwargs):
        url_name = resolve(request.path_info).url_name
        students = Student.objects.filter(**kwargs).select_related('mocktest1')

        if url_name == 'all_student_rest':
            serialized_data = StudentSerializer(instance=students, many=True)
        else:
            serialized_data = StudentSerializer(instance=students[0], many=False)

        return Response(serialized_data.data)

    def post(self, request, **kwargs):
        if resolve(request.path_info).url_name != 'all_student_rest':
            raise Http404

        student = StudentSerializer(data=request.data, context={'college_id': kwargs.get('college_id')})
        if student.is_valid():
            student.save()
        else:
            return Response(status=400)
        
        students = StudentSerializer(
            instance=Student.objects.filter(college_id=kwargs.get('college_id')).select_related('mocktest1'),
            many=True
        )

        return Response(students.data)

    def put(self, request, **kwargs):
        if resolve(request.path_info).url_name != 'all_student_rest':
            raise Http404

        student = Student.objects.filter(pk=request.data.get('id')).select_related('mocktest1')[0]
        student = StudentSerializer(instance=student, data=request.data)
        if student.is_valid():
            student.save()
        else:
            return Response(status=400)

        students = StudentSerializer(
            instance=Student.objects.filter(college_id=kwargs.get('college_id')).select_related('mocktest1'),
            many=True
        )

        return Response(students.data)

    def delete(self, request, **kwargs):
        if resolve(request.path_info).url_name != 'all_student_rest':
            raise Http404

        student = get_object_or_404(Student, pk=request.data.get('id'))
        student.delete()

        students = StudentSerializer(
            instance=Student.objects.filter(college_id=kwargs.get('college_id')).select_related('mocktest1'),
            many=True
        )

        return Response(students.data)
