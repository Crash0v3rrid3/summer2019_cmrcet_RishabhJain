from onlineapp.helpers import *
from django.contrib.auth.mixins import *
from django.contrib import messages


class AddStudentView(PermissionRequiredMixin, LoginRequiredMixin, View):
    permission_required = ('onlineapp.add_college', 'onlineapp.change_college', 'onlineapp.delete_college')
    def servePage(self, request, **kwargs):
        kwargs['child_template'] = get_template('add.html')
        return render(
            request,
            template_name='_base.html',
            context=kwargs
        )

    def get(self, request, **kwargs):
        student = None
        mocktest = None
        urlname = resolve(request.path_info).url_name
        if urlname == 'edit_student':
            student = get_object_or_404(Student, pk=kwargs.get('pk'))
            mocktest = get_object_or_404(MockTest1, student_id=kwargs.get('pk'))
            kwargs['title'] = 'Edit Student | Online App'
            kwargs['heading'] = 'Edit Student'
        elif urlname == 'delete_student':     # if Delete student, vulnerable!!!!
            student = get_object_or_404(Student, pk=kwargs.get('pk'))
            student.delete()
            return redirect('college_with_id', str(kwargs['college_id']))

        studentForm = StudentForm(instance=student)
        mockTestForm = MockTest1Form(instance=mocktest)


        if kwargs.get('title') is None:
            kwargs['title'] = 'Add Student | Online App'
            kwargs['heading'] = 'Add Student'

        kwargs = {
            'forms': [studentForm, mockTestForm],
            **kwargs,
        }

        return self.servePage(request, **kwargs)

    def post(self, request, **kwargs):
        urlname = resolve(request.path_info).url_name
        college = get_object_or_404(College, pk=kwargs.get('college_id'))
        student = None
        mocktest = None

        if urlname == 'delete_student':     # if Delete student
            student = get_object_or_404(Student, pk=kwargs.get('pk'))
            student.delete()
            return redirect('college_with_id ' + kwargs['college_id'])

        elif urlname == 'edit_student':     # if edit student
            student = get_object_or_404(Student, pk=kwargs.get('pk'))
            mocktest = get_object_or_404(MockTest1, student=student)
            kwargs['title'] = 'Edit College | Online App'
            kwargs['heading'] = 'Edit College'

                                            # else add student
        studentForm = StudentForm(
            data=request.POST,
            instance=student,
        )
        mockTestForm = MockTest1Form(
            data=request.POST,
            instance=mocktest,
        )

        messages.success(request, 'Successfully upated!')
        success = False

        if studentForm.is_valid():
            student = studentForm.save(commit=False)
            student.college = college
            student.save()
            studentForm.save()
            if mockTestForm.is_valid():
                mocktest = mockTestForm.save(commit=False)
                mocktest.total = sum(
                    map(
                        int,
                        [mockTestForm.data['problem1'],
                         mockTestForm.data['problem2'],
                         mockTestForm.data['problem3'],
                         mockTestForm.data['problem4']]
                    )
                )
                mocktest.student = student
                mocktest.save()
                mockTestForm.save()
                success = True

        kwargs['success'] = success
        return self.get(request, **kwargs)
