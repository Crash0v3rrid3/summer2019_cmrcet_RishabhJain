from django.template.loader import *
from django.http import *
from .models import *

def hello_world(req):
    hello = open('onlineapp/templates/hello_world.html')
    data = hello.read()
    hello.close()

    return HttpResponse(data)


def header_response(request):
    return HttpResponse(request.headers.get('foo'))

def get_college(request, college_id):
    try:
        students = Student.objects.filter(college=College.objects.get(pk=college_id), dropped_out=False).values('name', 'mocktest1__total')
        template = get_template('college_student_details.html')
        return HttpResponse(template.render(context={'students': students}))
    except:
        return None     # handle


def get_all_colleges(request):
    colleges = College.objects.values('id', 'name', 'acronym')
    template = get_template('records.html')
    return HttpResponse(template.render(context={'colleges': colleges}))
