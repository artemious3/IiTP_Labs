


import matplotlib.pyplot as plt 


def do(s, col):
    lines = open(s, 'r').readlines()

    vals = []
    for l in lines:
        iter, time = l.split()
        vals.append((int(iter), float(time)))

    vals.sort(key = lambda x : x[0])
    print(vals)

    a = list(zip(*vals))
    plt.plot(a[0], a[1], color=col)
    plt.xlabel("Number of iterations")
    plt.ylabel("Time elapsed (ns)")


do('results.txt', 'red')
# for i in range(16):
#     do("MCres{}.txt".format(i), 'red')
# for i in range(8):
#     do("NOMCres{}.txt".format(i), 'green')
plt.show()
