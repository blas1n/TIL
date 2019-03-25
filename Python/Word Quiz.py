import os
import sys
import random as r

def WriteFile(file_name, contents):
    file = open('C:/Users/user/Documents/Py_Class/' + file_name, 'a')

    for content in contents:
        file.write(content + '\n')

    file.close()

def ReadFile(file_name):
    file = open('C:/Users/user/Documents/Py_Class/' + file_name, 'r')
    text_str = file.read()
    file.close()
    return text_str

class QuestionManager:
    questions = {}

    def __init__(self):
        str = ReadFile('Question.txt')
        str_list = str.split()

        for i in range(0, len(str_list), 2):
            self.questions[str_list[i]] = str_list[i + 1]

    def StartQuestion(self):
        num = int(input('풀 문제의 개수. '))
        
        while num > 0:
            key = r.choice(list(self.questions.keys()))

            while True:
                awnser = input(key + ' ')

                if self.questions[key] == awnser:
                    print('정답\n')
                    break

                else:
                    print('오답\n')

            num -= 1

        print('끝')

    def AddQuestion(self):
        lists = []
        num = int(input('입력할 단어 수. '))
        for i in range(num):
            lists.append(input())
        WriteFile('Question.txt', lists)

        for element in lists:
            tmp = element.split()
            self.questions[tmp[0]] = tmp[1]

if __name__ == '__main__':
    tutorial = ReadFile('StartText.txt')
    q = QuestionManager()

    while True:
        print(tutorial)
        choice = int(input('원하는 명령 입력. '))

        os.system('cls')

        if (choice == 1):
            q.StartQuestion()
        elif (choice == 2):
            q.AddQuestion()
        elif (choice == 3):
            break

        os.system('cls')