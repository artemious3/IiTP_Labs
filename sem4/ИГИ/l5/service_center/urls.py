


from django.urls import path

from .views import *

urlpatterns = [
        path('profile', redirect_to_user_index, name='redirect'),

        path('client/', client_index, name='client_index'),
        path('orders/approved', orders_approved_view, name='orders_approved'),
        path('orders/unapproved', orders_unapproved_view, name='orders_unapproved'),
        path('orders/<int:order_id>/submit/', order_submit_view, name='order_submit'),
        path('orders/<int:pk>/delete/', OrderDeleteView.as_view(), name='order_delete'),
        path('order/new/', OrderNewView.as_view(), name='order_new'),
        path('order/<int:order_id>/add-service/', AddServiceView.as_view(), name='order_service_add'),
        path('orders/<int:pk>/update-service/', OrderServiceUpdateView.as_view(), name='order_service_update'),
        path('orders/<int:pk>/delete-service/', OrderServiceDeleteView.as_view(), name='order_service_delete'),


        path('staff/index', staff_index,  name='staff_index'),
        path('staff/orders/list-appr', staff_orders_approved_view,  name='staff_orders_appr'),
        path('staff/orders/list-unappr', staff_orders_unapproved_view,  name='staff_orders_unappr'),
        path('staff/order/<int:pk>/complete/', order_complete_view, name='staff_complete'),
        path('staff/order/<int:pk>/sp/add', AddSparePartView.as_view(), name='staff_sp_add'),
        path('staff/order/sp/<int:pk>/update', SparePartUpdateView.as_view(), name='staff_sp_update'),
        path('staff/order/sp/<int:pk>/delete', SparePartDeleteView.as_view(), name='staff_sp_delete'),

        path('staff/client/<int:pk>', client_info_view, name='staff_client_info'),
        

    ]
