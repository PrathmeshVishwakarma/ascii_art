# chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "

# test = [1,2,3,4,5]
# lol = [7,8,9]
# test.append(lol)
# print(test)
# test.extend(lol)
# print(test)

# x = 0
# for i in range(0, 256, 4):
#     print(f"iteration: {x}\t", i, i+1, i+2, i+3)
#     x += 1 



import string
import random

lst = string.ascii_letters + string.digits
lst = [i for i in lst]

random.shuffle(lst)
random.shuffle(lst)
random.shuffle(lst)

file = open("usernames.txt", "w+")

for i in range(1000):
    username = []
    for i in range(5):
        username.append(lst[random.randint(0, 61)])
    random.shuffle(username)
    username = "0x" + "".join(i for i in username)
    file.write(username+"\n")

file = open("usernames.txt", "w+")
usernames = file.read().split("\n")
for i in range(1):
    print(usernames[random.randint(0, 1000)])
