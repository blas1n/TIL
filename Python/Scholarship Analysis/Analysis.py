# 1. 금액
# 2. 건수
# 3. 1건 당 금액

import numpy as np
import pandas as pd
from matplotlib import font_manager, rc
import matplotlib.pyplot as plt

def init():
    font_name = font_manager.FontProperties(fname="C:/Windows/Fonts/NanumBarunGothic.ttf").get_name()
    rc('font', family=font_name)

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

def graph1():
    plt.plot([x for n, x in enumerate(mba[0]) if n % 3 == 1], label=r'1학기')
    plt.plot([x for n, x in enumerate(mba[0]) if n % 3 == 2], label=r'2학기')

    plt.title('국가 우수 장학금 학기 별 비교')
    plt.xlabel('연도')
    plt.ylabel('총 금액')
    plt.grid(True)
    plt.legend(loc='upper left')
    plt.tight_layout()
    plt.show()

def graph2():
    plt.plot([x for n, x in enumerate(nba[0]) if n % 3 == 1], label=r'1학기')
    plt.plot([x for n, x in enumerate(nba[0]) if n % 3 == 2], label=r'2학기')

    plt.title('소득 연계 장학금 학기 별 비교')
    plt.xlabel('연도')
    plt.ylabel('총 금액')
    plt.grid(True)
    plt.legend(loc='upper left')
    plt.tight_layout()
    plt.show()

def graph3():
    plt.plot([x for n, x in enumerate(wba[0]) if n % 3 == 1], label=r'1학기')
    plt.plot([x for n, x in enumerate(wba[0]) if n % 3 == 2], label=r'2학기')

    plt.title('취업 연계 장학금 학기 별 비교')
    plt.xlabel('연도')
    plt.ylabel('총 금액')
    plt.grid(True)
    plt.legend(loc='upper left')
    plt.tight_layout()
    plt.show()

def graph4():
    plt.plot([x for n, x in enumerate(mba[1]) if n % 3 == 0], label=r'국가 우수 장학금')
    plt.plot([x for n, x in enumerate(nba[1]) if n % 3 == 0], label=r'소득 연계 장학금')
    plt.plot([x for n, x in enumerate(wba[1]) if n % 3 == 0], label=r'취업 연계 장학금')

    plt.title('장학금 별 지원 건수')
    plt.xlabel('연도')
    plt.ylabel('건수')
    plt.grid(True)
    plt.legend(loc='upper left')
    plt.tight_layout()
    plt.show()

def graph5():
    plt.plot([x for n, x in enumerate(mba[2]) if n % 3 == 0], label=r'국가 우수 장학금')
    plt.plot([x for n, x in enumerate(nba[2]) if n % 3 == 0], label=r'소득 연계 장학금')
    plt.plot([x for n, x in enumerate(wba[2]) if n % 3 == 0], label=r'취업 연계 장학금')

    plt.title('장학금 별 지원 금액')
    plt.xlabel('연도')
    plt.ylabel('금액')
    plt.grid(True)
    plt.legend(loc='upper left')
    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    init()
    for i in range(6):
        exec(f'graph{i}()')