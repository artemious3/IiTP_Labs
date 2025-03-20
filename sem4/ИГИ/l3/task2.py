

import inputils 



def process(lst : list[int]):
    """
    Find number of positive integers in list
        @lst : list to process
    """
    return sum([i > 0 for i in lst])

def inp():
    """
    Input the list of integers until `10` is entered
    """
    print("Input n")
    lst = []
    i = inputils.getint("|>")
    while i != 10:
        lst.append(i)
        i = inputils.getint("|>")
    lst.append(10)
    return lst

def run():
    """Run task 2"""
    print("Input the integer numbers. After you input `10`,")
    print("the number of positive numbers in the list will be shown")
    lst = inp()
    print("\n\nNumber of i > 0 : {}".format(process(lst)))

