

from collections.abc import Callable
import task1
import task2
import task3
import task4
import task5


GREETING =  """
IGI
LABORATORY WORK 3
AUTHOR : Подгайский Артём
VARIANT : 3
GROUP : 353501
"""


MENU = """
Please, input action
     `1` -- Task 1 : power series
     `2` -- Task 2 : count positive numbers
     `3` -- Task 3 : check if string is hex number 
     `4` -- Task 4 : analyze hard-coded string
     `5` -- Task 5 : operations with real list
"""



def catch_ctrl_c(f : Callable):
    """Catch CTRL-C input to provide more user friendly output to user"""
    def func():
        try:
            f()
        except KeyboardInterrupt:
            print("\n\nGood bye!")
    return func


@catch_ctrl_c
def main():
    print(GREETING)
    while True:
        print(MENU)
        a = input(">>> ")
        if a == '1':
            task1.run()
        elif a == '2':
            task2.run()
        elif a == '3':
            task3.run()
        elif a == '4':
            task4.run()
        elif a == '5':
            task5.run()
        else:
            print("Wrong input")




if __name__ == "__main__":
    main()
