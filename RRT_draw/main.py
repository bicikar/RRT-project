from PIL import Image, ImageDraw
import os

if __name__ == '__main__':
    path = os.path.realpath(os.path.join(os.path.dirname(__file__), '..',
                                         'RRT_linux/cmake-build-debug',
                                         'OUTPUT.txt'))
    file = open(path)
    _, rows = file.readline().split()
    rows = int(rows)
    _, cols = file.readline().split()
    cols = int(cols)
    _, _, x_start, y_start = file.readline().split()
    x_start = int(x_start)
    y_start = int(y_start)
    _, _, x_end, y_end = file.readline().split()
    x_end = int(x_end)
    y_end = int(y_end)
    file.readline()
    file.readline()
    file.readline()
    grid = [[0] * cols for i in range(rows)]

    for row, data in enumerate(file.readlines()):
        for col, symbol in enumerate(data):
            if symbol != '\n':
                grid[col][row] = symbol

    image = Image.new("RGB", (cols, rows), (255, 255, 255))
    draw = ImageDraw.Draw(image)
    for i in range(rows):
        for j in range(cols):
            if grid[i][j] == '.':
                draw.point((i, j), (255, 255, 255))
            if grid[i][j] == '@':
                draw.point((i, j), (70, 70, 70))
            if grid[i][j] == '*':
                draw.point((i, j), (0, 0, 0))
            if grid[i][j] == 'A':
                draw.point((i, j), (0, 130, 13))

    rad = min(rows, cols) / 125
    for i in range(rows):
        for j in range(cols):
            if grid[i][j] == 'M':
                draw.ellipse((i - rad, j - rad, i + rad, j + rad), fill=(30, 170, 10))
    big_r = rad * 1.25
    draw.ellipse((x_start - big_r, y_start - big_r, x_start + big_r, y_start + big_r),
                 fill='red')
    draw.ellipse((x_end - big_r, y_end - big_r, x_end + big_r, y_end + big_r),
                 fill='blue')
    image.save('res.png')

    file.close()
