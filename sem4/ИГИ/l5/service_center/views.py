from django.db.models.base import pre_init
from django.middleware import csrf
from django.http import HttpResponse, HttpResponseForbidden, HttpResponseRedirect
from django.shortcuts import get_object_or_404, render
from django.contrib.auth.decorators import login_required, permission_required
from django.contrib.auth.mixins import LoginRequiredMixin, PermissionRequiredMixin
from django.urls import reverse, reverse_lazy
from django.views.generic import CreateView, DeleteView, UpdateView
from django.views.decorators.csrf import requires_csrf_token
from django.core.exceptions import PermissionDenied

from service_center.forms import AddServiceForm, AddSparePartForm, NewOrderForm
from .models import Client, Employee, Order, OrderService, OrderSpareParts

###################################################################
####################  COMMON ######################################
###################################################################


def redirect_to_user_index(req):
    if req.user.groups.filter(name='Employee').exists():
        return HttpResponseRedirect(reverse_lazy('staff_index'))
    elif req.user.groups.filter(name='Client').exists():
        return HttpResponseRedirect(reverse_lazy('client_index'))
    else:
        return HttpResponseForbidden()


###################################################################
####################  CLIENT STUFF  ###############################
###################################################################

@login_required
@permission_required('service_center.client_perm', raise_exception=True)
def client_index(req):
    return render(req, 'service_center/client/client_index.html', {"user":req.user})



@login_required
@permission_required('service_center.client_perm', raise_exception=True)
def orders_unapproved_view(req):
   user = req.user 
   client = Client.objects.get(user=user)
   orders = Order.objects.filter(client=client)
   submitted_orders = orders.filter(submitted=True)
   unsubmitted_orders = orders.filter(submitted=False)
   return render(req, 'service_center/client/orders_unapproved.html', {"submitted_orders":submitted_orders, 
                                                         "unsubmitted_orders":unsubmitted_orders})


@login_required
@permission_required('service_center.client_perm', raise_exception=True)
def orders_approved_view(req):
   user = req.user 
   client = Client.objects.get(user=user)
   orders = Order.objects.filter(client=client)
   submitted_orders = orders.filter(submitted=True)
   unsubmitted_orders = orders.filter(submitted=False)
   return render(req, 'service_center/client/orders_approved.html', {"submitted_orders":submitted_orders, 
                                                     "unsubmitted_orders":unsubmitted_orders})


# @requires_csrf_token
@login_required
@permission_required('service_center.client_perm', raise_exception=True)
def order_submit_view(req, order_id):
    order = get_object_or_404(Order,pk=order_id)
    client = Client.objects.get(user=req.user)
    if order.client != client:
        return HttpResponseForbidden();

    if req.method=="POST":
        if not order.order_services.exists():
            return render(req, 'service_center/client/order_cant_submit.html')
        order.submitted = True
        order.save()
        return HttpResponseRedirect(reverse_lazy('orders_unapproved'))
    else:
        return render(req, 'service_center/client/order_submit.html', {"order": order,
                                                                "csrf_token":csrf.get_token(req)})


class OrderDeleteView(DeleteView, LoginRequiredMixin, PermissionRequiredMixin):
    model = Order
    permission_required = 'service_center.client_perm'
    template_name = 'service_center/client/order_delete.html'
    success_url = reverse_lazy('orders_unapproved')

    def dispatch(self, request, *args, **kwargs):
        order_id = self.kwargs.get('pk')
        self.order = get_object_or_404(Order, id=order_id)
        if self.order.client != self.request.user.client:
            raise PermissionDenied("You have no access to this order")
        return super().dispatch(request, *args, **kwargs)


class OrderNewView(CreateView, LoginRequiredMixin, PermissionRequiredMixin):
    form_class = NewOrderForm
    permission_required = 'service_center.client_perm'
    template_name = 'service_center/client/order_create.html'
    success_url = reverse_lazy('orders_unapproved')

    def get_form_kwargs(self):
        kwargs = super(OrderNewView, self).get_form_kwargs()
        kwargs['client'] = Client.objects.get(user=self.request.user)
        return kwargs


class OrderServiceUpdateView(UpdateView, LoginRequiredMixin, PermissionRequiredMixin):
    model = OrderService
    permission_required = 'service_center.client_perm'
    template_name = 'service_center/client/services_update.html'
    fields = ["number"]
    success_url = reverse_lazy('orders_unapproved')

    # TODO : check if order belongs to user

class OrderServiceDeleteView(DeleteView, LoginRequiredMixin, PermissionRequiredMixin):
    model = OrderService
    permission_required = 'service_center.client_perm'
    template_name = 'service_center/client/services_delete.html'
    success_url = reverse_lazy('orders_unapproved')

    # TODO : check if order belongs to user



class AddServiceView(CreateView, LoginRequiredMixin, PermissionRequiredMixin):
    form_class = AddServiceForm
    permission_required = 'service_center.client_perm'
    template_name = 'service_center/client/service_add.html'
    success_url = reverse_lazy('orders_unapproved')

    def dispatch(self, request, *args, **kwargs):
        order_id = self.kwargs.get('order_id')
        self.order = get_object_or_404(Order, id=order_id)
        if self.order.client != self.request.user.client:
            raise PermissionDenied("You have no access to this order")
        if self.order.approved:
            raise PermissionDenied("Approved orders can not be changed")
        
        return super(AddServiceView, self).dispatch(request, *args, **kwargs)

    def get_form_kwargs(self):
        kwargs = super(AddServiceView, self).get_form_kwargs()
        kwargs['order'] = self.order 
        return kwargs



###################################################################
####################  STAFF STUFF  ################################
###################################################################

@login_required
@permission_required('service_center.employee_perm', raise_exception=True)
def staff_orders_unapproved_view(req):
    employee = Employee.objects.get(user=req.user)
    orders_subm = Order.objects.filter(submitted=True, approved=False, employee=employee).order_by('date_scheduled')
    return render(req, 'service_center/staff/orders_unapproved_view.html', {"orders":orders_subm})

@login_required
@permission_required('service_center.employee_perm', raise_exception=True)
def staff_orders_approved_view(req):
    employee = Employee.objects.get(user=req.user)
    orders_subm = Order.objects.filter(submitted=True, approved=True, employee=employee).order_by('-date_scheduled')
    return render(req, 'service_center/staff/orders_approved_view.html', {"orders":orders_subm})

@login_required
@permission_required('service_center.employee_perm', raise_exception=True)
def staff_index(req):
    return render(req, 'service_center/staff/index.html')


@login_required
@permission_required('service_center.employee_perm', raise_exception=True)
def order_complete_view(req, pk):
    order = get_object_or_404(Order,pk=pk)
    employee = Employee.objects.get(user=req.user)
    if order.employee != employee:
        return HttpResponseForbidden();

    if req.method=="POST":
        order.approved = True
        order.save()
        return HttpResponseRedirect(reverse_lazy('staff_orders_unappr'))
    else:
        return render(req, 'service_center/staff/order_complete.html', {"order": order,
                                                                    "csrf_token":csrf.get_token(req)})

class SparePartUpdateView(UpdateView, LoginRequiredMixin, PermissionRequiredMixin):
    model = OrderSpareParts
    permission_required = 'service_center.client_perm'
    template_name = 'service_center/staff/order_spare_part_change.html'
    fields = ["number"]
    success_url = reverse_lazy('staff_orders_unappr')

    # TODO : check if order belongs to user

class SparePartDeleteView(DeleteView, LoginRequiredMixin, PermissionRequiredMixin):
    model = OrderSpareParts
    permission_required = 'service_center.client_perm'
    template_name = 'service_center/staff/order_spare_part_delete.html'
    success_url = reverse_lazy('staff_orders_unappr')

    # TODO : check if order belongs to user



class AddSparePartView(CreateView, LoginRequiredMixin, PermissionRequiredMixin):
    form_class = AddSparePartForm
    permission_required = 'service_center.client_perm'
    template_name = 'service_center/staff/order_spare_part_add.html'
    success_url = reverse_lazy('staff_orders_unappr')

    def dispatch(self, request, *args, **kwargs):
        order_id = self.kwargs.get('pk')
        self.order = get_object_or_404(Order, id=order_id)
        if self.order.employee != self.request.user.employee:
            raise PermissionDenied("You have no access to this order")
        if self.order.approved:
            raise PermissionDenied("Approved orders can not be changed")
        
        return super(AddSparePartView, self).dispatch(request, *args, **kwargs)

    def get_form_kwargs(self):
        kwargs = super(AddSparePartView, self).get_form_kwargs()
        kwargs['order'] = self.order 
        return kwargs


def client_info_view(req, pk):
    client = Client.objects.get(pk=pk)
    return render(req, 'service_center/staff/client_info.html', {"client":client})


# def create_order_view(req):


