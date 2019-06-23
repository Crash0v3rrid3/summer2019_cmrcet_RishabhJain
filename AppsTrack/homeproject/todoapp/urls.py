from django.urls import path
from . import views

urlpatterns = [
    path('login/', views.LoginView.as_view(), name='login'),
    path('signup/', views.RegisterView.as_view(), name='signup'),
    path('logout/', views.logout_user, name='logout'),
    path('lists/', views.ToDoListView.as_view(), name='all_lists'),                    # All lists
    path('lists/add/', views.ToDoListView.as_view(), name='add_list'),                 # List add
    path('lists/<int:pk>/edit/', views.ToDoListView.as_view(), name='edit_list'),      # List edit
    path('lists/<int:pk>/delete/', views.ToDoListView.as_view(), name='delete_list'),  # List delete
    path('lists/<int:pk>/', views.ToDoItemView.as_view(), name='all_items'),           # Items in a specific list
    path('lists/<int:list_id>/add/', views.ToDoItemView.as_view(), name='add_item'),   # Add Item to a specific list
    path('lists/<int:list_id>/edit/<int:pk>/', views.ToDoItemView.as_view(), name='edit_item'),      # Items in a specific list
    path('lists/<int:list_id>/delete/<int:pk>/', views.ToDoItemView.as_view(), name='delete_item'),  # Items in a specific list
    #path('lists/<int:list_id>/items/<int:pk>/', views, name='specific_item'),          # Item of a specific list
]