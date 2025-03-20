
from collections.abc import Callable

def input_until_valid(func : Callable):
    """
    Decorator to continue the input while exceptions are thrown
    """
    def inp(invitation : str):
        i = func()
        s = ""
        while True: 
            try:
                s = input(invitation)
                i = func(s)
                return i
            except ValueError: 
                print("Wrong input\n")
    return inp

def input_until_valid_wrap(func : Callable):
    """
    Decorator to continue the input while exceptions are thrown and values are not in desired range
    """
    def inp(invitation : str, minv, maxv):
        s = ""
        while True: 
            try:
                s = input(invitation)
                i = func(s)
                if i < minv or i > maxv:
                    print("Value should be in range {}..{}".format(minv, maxv))
                    raise ValueError
                return i
            except ValueError: 
                print("Wrong input\n")
    return inp



def input_list(func : Callable):
    """
    Decorator to input the list of elements until empty input
    """
    def inp(invitation : str):
        lst = []
        s = ""
        while True:
            try:
                s = input("[{}]{}".format(len(lst), invitation))
                if s == "":
                    break;
                lst.append(func(s))
            except ValueError: 
                print("Wrong input\n")
        return lst
    return inp




getint = input_until_valid(int)

getint_wrap = input_until_valid_wrap(int)

getfloat = input_until_valid(float)

getfloat_wrap = input_until_valid_wrap(float)

getfloat_list = input_list(float)


