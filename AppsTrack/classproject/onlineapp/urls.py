from django.urls import path
from . import views

urlpatterns = [
    path('test_path/', views.test_path, name='test_path'),
    path('colleges/', views.CollegeView.as_view(), name='all_colleges'),
    path('colleges/add', views.AddCollegeView.as_view(), name='add_college'),
    path('colleges/<int:pk>/edit', views.AddCollegeView.as_view(), name='edit_college'),
    path('colleges/<int:pk>/delete', views.AddCollegeView.as_view(), name='delete_college'),
    path('colleges/<int:college_id>', views.CollegeView.as_view(), name='college_with_id'),
    path('colleges/<str:college__acronym>', views.CollegeView.as_view(), name='college_with_acronym'),
    path('colleges/<int:college_id>/add', views.AddStudentView.as_view(), name='add_student'),
    path('colleges/<int:college_id>/edit/<int:pk>', views.AddStudentView.as_view(), name='edit_student'),
    path('colleges/<int:college_id>/delete/<int:pk>', views.AddStudentView.as_view(), name='delete_student'),
    path('api/v1/colleges/', views.rest_colleges, name='all_colleges_rest'),
    path('api/v1/colleges/<int:pk>/', views.rest_colleges, name='college_rest'),
    path('api/v1/colleges/<int:college_id>/students/', views.ListStudent.as_view(), name='all_student_rest'),
    path('api/v1/colleges/<int:college_id>/students/<int:pk>', views.ListStudent.as_view(), name='student_rest'),
    path('api/v1/token', views.generate_token)
]