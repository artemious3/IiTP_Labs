


import matplotlib.pyplot as plt 


def do(s):
    lines = open(s, 'r').readlines()

    vals = []
    for l in lines:
        iter, time, res = l.split()
        vals.append((int(iter), int(time)))

    vals.sort(key = lambda x : x[0])
    print(vals)

    a = list(zip(*vals))
    plt.plot(a[0], a[1])
    plt.xlabel("Number of iterations")
    plt.ylabel("Time elapsed")


# do('results_STM.txt')
for i in range(16):
    do("MCres{}.txt".format(i))
# for i in range(16):
#     do("NOMCres{}.txt".format(i))
plt.show()
