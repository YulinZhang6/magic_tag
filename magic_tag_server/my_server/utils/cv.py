import random

import cv2
import numpy as np
from PIL import ImageFont, ImageDraw, Image
from io import BytesIO
from django.conf import settings

# 返回200*200数组，0 255
from django.core.files.uploadedfile import InMemoryUploadedFile


def dithering(img_data):
    '''
    图片抖动，输入图片路径
    '''
    # img_gray0 = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)

    img_gray0 = cv2.imdecode(np.asarray(bytearray(img_data.read()), dtype='uint8'), cv2.IMREAD_GRAYSCALE)
    if img_gray0 is None:
        return
    # img_gray0 = 255 - img_gray0
    img_gray0 = cv2.resize(img_gray0, (200, 200))
    h, w = img_gray0.shape

    img_dither = np.zeros((h + 1, w + 1), dtype=np.float)
    for i in range(h):
        for j in range(w):
            img_dither[i, j] = img_gray0[i, j]

    threshold = 128

    for i in range(h):
        for j in range(w):
            old_pix = img_dither[i, j]
            if (img_dither[i, j] > threshold):
                new_pix = 255
            else:
                new_pix = 0

            img_dither[i, j] = new_pix
            quant_err = old_pix - new_pix

            if j > 0:
                img_dither[i + 1, j - 1] = img_dither[i + 1, j - 1] + quant_err * 3 / 16
            img_dither[i + 1, j] = img_dither[i + 1, j] + quant_err * 5 / 16
            img_dither[i, j + 1] = img_dither[i, j + 1] + quant_err * 7 / 16
            img_dither[i + 1, j + 1] = img_dither[i + 1, j + 1] + quant_err * 1 / 16

    img_dither = img_dither.astype(np.uint8)
    img_dither = img_dither[0:h, 0:w]
    r = random.randint(0, 100000)
    img_bytestream = np.array(cv2.imencode('.bmp', img_dither)[1]).tobytes()
    return InMemoryUploadedFile(BytesIO(img_bytestream), None, '{}.bmp'.format(r), None, None, None)


def cut(obj, sec):
    '''
    分割字符串, sec 每段长度
    '''
    return [obj[i:i + sec] for i in range(0, len(obj), sec)]

# 200*200 图像-> 6667 长度字符串 834*7 + 829
def img_to_base64(img):

    bt = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
          'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
          'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
          'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/']
    bits = ''
    cnt = 0
    ch = 0
    h, w = img.shape
    for i in range(h):
        for j in range(w):
            cnt += 1
            if img[i, j] > 0:
                ch += 1
            ch << 1
            if cnt == 6:
                bits += bt[ch]
                ch = 0
                cnt = 0
    bits += bt[ch]
    print(len(bits))
    return bits

# 200*200 图像-> 5000 bit(10000字符) 200字符 * 50
def img_to_hex(img):
    cv2.flip(img,1,img)
    bt = '0123456789ABCDEF'
    bits = ''
    cnt = 0
    ch = 0
    h, w = img.shape
    for i in range(h):
        for j in range(w):
            cnt += 1
            if img[i, j] > 0:
                ch += 1
            ch <<= 1
            if cnt == 4:
                bits += bt[ch >> 1]
                ch = 0
                cnt = 0
    print(len(bits))
    return cut(bits, 200)

# 返回字符长度，ASCII 10px，其他20px
def charSize(char):
    if len(char) != 1:
        return 20
    if ord(char) >= ord(' ') and ord(char) <= ord('}'):
        return 10
    else:
        return 20

def textProcess(text):
    line_len = 0
    line_max = 190
    out = ''
    for ch in text:
        if ch == '\n':
            out += ch
            line_len = 0
            continue
        if line_len + charSize(ch) > line_max:
            # 超出限定长度
            out += '\n' + ch
            line_len = charSize(ch)
        else:
            out += ch
            line_len += charSize(ch)
    return out


# 显示文字，返回图片数组
def drawText(text, org=(5, 5)):
    text = textProcess(text)
    img = np.asfarray([[255] * 200] * 200)
    img_pil = Image.fromarray(img)
    draw = ImageDraw.Draw(img_pil)
    color = (0,)
    filepath = settings.BASE_DIR+'/my_server/utils/font/simhei.ttf'
    print(filepath)
    font1 = ImageFont.truetype(filepath, 20, encoding="utf-8")
    # font2 = ImageFont.truetype("font/arialbd.ttf", 16, encoding="utf-8")

    draw.text(org, text, color, font1, spacing=10)
    # draw.text((85, 22), "15358059674", color, font2, spacing=1)

    img = np.array(img_pil, dtype=np.float)
    # print(img)
    # draw.show()
    # cv2.putText(img, text, org, fontFace, 1, color, 1, 4)
    r = random.randint(0, 100000)
    img_bytestream = np.array(cv2.imencode('.bmp', img)[1]).tobytes()
    return InMemoryUploadedFile(BytesIO(img_bytestream), None, '{}.bmp'.format(r), None, None, None)


if __name__ == '__main__':
    # # img_dither = dithering('pics/0.png')
    # text = '姓名：张三\n电话：12345678987 123456789785155'
    # img = drawText(text)
    # cv2.imshow("dithering", img)
    #
    # # bits = img_to_hex(img_dither)
    #
    # # print(len(bits))
    # # cv2.imshow("img1", img1)
    # cv2.waitKey(0)
    f = open('cv_copy.py', 'rb')
    print(type(f.read()))