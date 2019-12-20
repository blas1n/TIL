# 1. 금액
# 2. 건수
# 3. 1건 당 금액

import numpy as np
import pandas as pd
from matplotlib import font_manager, rc
import matplotlib.pyplot as plt

mba = pd.read_csv('Mertit_Based_Aid.csv').T
nba = pd.read_csv('Need_Based_Aid.csv').T
wba = pd.read_csv('Work_Based_Aid.csv').T

def graph0():
    plt.plot([x for n, x in enumerate(mba[0]) if n % 3 == 0], label=r'국가 우수 장학금')
    plt.plot([x for n, x in enumerate(nba[0]) if n % 3 == 0], label=r'소득 연계 장학금')
    plt.plot([x for n, x in enumerate(wba[0]) if n % 3 == 0], label=r'취업 연계 장학금')

    plt.title('연도 별 금액')
    plt.xlabel('연도')
    plt.ylabel('총 금액')
    plt.grid(True)
    plt.legend(loc='upper left')
    plt.tight_layout()
    plt.show()



if __name__ == '__main__':
    font_name = font_manager.FontProperties(fname="C:/Windows/Fonts/NanumBarunGothic.ttf").get_name()
    rc('font', family=font_name)
    for i in range(1):
        exec(f'graph{i}()')