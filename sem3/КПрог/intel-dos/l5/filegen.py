
import random
import string


el = 0


def rands():
    global el
    isne = random.randint(0,1)
    if isne:
        return ''.join([random.choice(string.ascii_uppercase) for _ in range(random.randint(1,1000))])+'\n' 
    else:
        el = el + 1
        return '\n'

bytes_written = 0
with open('test.txt', 'w') as f:
    while bytes_written < 64000:
        s = rands()
        bytes_written += len(s)
        f.write(s)
        
print("EMPTY LINES : ", el)
        