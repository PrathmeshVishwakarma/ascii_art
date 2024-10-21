from PIL import Image, ImageDraw, ImageFont
import math

image = Image.open("0xq4G7i.png")
image = image.resize((150, 35))
# chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "[::-1]
chars_original = [" ", '.',',',':',';','+','*','?','%','S','#','@'][::-1]
# chars = ['@',' ',':',';','+','*','?','%','S','#','@']
image = image.convert("L")
width, height = image.size
pixels = image.load()

print(height, width)

def get_char(x):
    try:
        return chars_original[x//23]
    except:
        return chars_original[0]

file = open("0xq4G7i.txt", "w")


for i in range(height):
    row = []
    for j in range(width):
        file.write(get_char(pixels[j, i]))
    file.write("\n")



# image.save("result.jpg")
