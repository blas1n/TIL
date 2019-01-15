# -*- coding: utf-8 -*-
# UTF-8 encoding when using korean

# 정형화된 문자열의 예시
# Canvas size: 600 x 400
# Background color: 0xfa023d
# Number of frames: 5
# No.: width height alpha duration
# 1: 360 360 yes 150
# 2: 360 360 yes 120
# 3: 360 360 yes 110
# 4: 360 360 no 140
# 5: 360 360 yes 115

def Extraction(inputList):
    DelInput = (0, 1, 3, 5, 6, 8, 9, 10, 12, 13, 14, 15, 16, 17, 22, 27, 32, 37)

    for i in DelInput:
        inputList[i] = ' '

    while inputList.count(' ') != 0:
        inputList.remove(' ')

def HexToInt(inputList):
    hexVal = Search(inputList, '0x')
    inputList[hexVal] = str(int(inputList[hexVal], 16))

def Search(inputList, key):
    for i in range(len(inputList)):
        if key in inputList[i]:
            return i

    return -1

def OrderChange(inputList):
    while True:
        boolVal = Search(inputList, 'yes')
    
        if (boolVal == -1):
            boolVal = Search(inputList, 'no')

        if (boolVal == -1):
            return -1

        tempList = inputList[boolVal].replace('yes', '1')
        if tempList == 'no':
            tempList = inputList[boolVal].replace('no', '0')

        inputList[boolVal] = tempList

        Swap(inputList, boolVal, boolVal + 1)

def Swap(inputList, Pos1, Pos2):
    temp = inputList[Pos1]
    inputList[Pos1] = inputList[Pos2]
    inputList[Pos2] = temp

def TieResolution(inputList):
    for i in range(4, 21, 4):
        inputList[i] += 'x' + inputList[i + 1] + ', ' + inputList[i + 2] + ', ' + inputList[i + 3]
    
    for i in range(5, 10):
        del inputList[i:i+3]

if __name__ == '__main__':
    inputList = []

    # 입력 받는 부분
    for i in range(9):
        userInput = input()
        inputList.extend(userInput.split())

    # 처리 부분 (필요 없는 부분 제거, 16 진수 -> 10 진수,
    #           yes / no와 수의 순서 변경, 해상도 묶기)
    Extraction(inputList)
    HexToInt(inputList)
    OrderChange(inputList)
    TieResolution(inputList)

    for result in inputList:
        print(result)