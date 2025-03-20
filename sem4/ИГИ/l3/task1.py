
import math as m 
import inputils


def calc(x : float, eps : float ):
    """
    Calculate tailor series for ln(1+x)
        @x : argument
        @eps : desired error
    """
    n = 1
    xm = x 
    acc = xm/n
    term = x
    while abs(term) > eps and n < 500:
        n = n+1
        xm = xm * x * -1
        term = xm/n
        acc = acc + term
    return (n, acc)


def run():
    """ Run task 1 """
    print("Calculate ln(1+x) as Tailor series")
    x = inputils.getfloat_wrap("x : ", -1.0, 1.0)
    eps = inputils.getfloat_wrap("eps : ", 0.0, 1.0)
    n, val = calc(x, eps)

    print("|{:^9}|{:^3}|{:^15}|{:^15}|{:^15}|".format("x", "n", "f(x)", "math.f(x)", "eps"))
    print("|{:^9}|{:^3}|{:^15.10}|{:^15.10}|{:^15.10}|".format(x, n, val, m.log(1+x),eps))
    
