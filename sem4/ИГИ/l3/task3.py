

def process(s : str):
    """Test if @s can represent hexadecimal number"""
    s = s.strip()
    s = s.removeprefix("0x")
    for c in s.lower(): 
        if not ((c >= '0' and c <= '9') or
        (c >= 'a' and c <= 'f')):
                return False 
    return True


def run():
    """Run task 3"""
    s = input("Input string to be tested if it's hex number:\n")
    print("Is hex number : {}".format(process(s)))


