
import inputils as inp

def max_abs(l : list[float]):
    """Return element with maximum absolute value"""
    return max(l, key = lambda x : abs(x)) if l != [] else None


def sum_before_last_positive(l : list[float]):
    """Return sum of all elements before last positive in the list"""
    idxes = [i for i,v in enumerate(l) if v > 0]
    idx = None if idxes == [] else idxes[-1]
    if idx == None or idx == 0:
        return None
    return sum(l[:idx])

def run():
    """Run task 5"""
    print("Input the list of real values")
    print("Input empty string to finish")
    l = inp.getfloat_list(">>> ")
    print("")


    print(l)
    print("\n_____________")
    print("Max abs element : ", max_abs(l))
    print("Sum before last positive : ", sum_before_last_positive(l))
    print("______________")
