import pymysql
from enum import Post

# 데이터 베이스에 접속하는 함수
def get_connection() :
    conn = pymysql.connect(host='127.0.0.1', user='root',
            password='kdhas2!)', db='python_ex_db', charset='utf8')

    return conn

def get_post_list(pivot) :
    conn = get_connection()

    sql = 'select * from post order by %s desc limit 3'

    cursor = conn.cursor()
    cursor.execute(sql, (pivot))
    result = cursor.fetchall()

    result = []
    for row in result :
        temp_dic = {'id' : row[0], 'title' : row[1], 'content' : row[2], 'upload' : row[3], 'like' : row[4], 'picture' : row[5] }
        result.append(temp_dic)

    conn.close()
    return result

# 학생 정보를 저장한다.
def add_student(stu_name, stu_age, stu_addr) :
    # 쿼리문
    sql = '''insert into student_table
             (stu_name, stu_age, stu_addr)
             values (%s, %s, %s)'''
    # 데이터 베이스 접속
    conn = get_connection()

    # 쿼리 실행
    cursor = conn.cursor()
    cursor.execute(sql, (stu_name, stu_age, stu_addr))
    conn.commit()

    # 방금 저장한 학생의 번호를 가져온다.
    sql2 = 'select max(stu_idx) from student_table'
    cursor.execute(sql2)
    result = cursor.fetchone()
    idx = result[0]

    # 접속 종료
    conn.close()

    return idx

# 학생 한명의 정보를 가져오는 함수
def get_student_info(stu_idx) :
    # 쿼리문
    sql = '''select stu_name, stu_age, stu_addr
             from student_table
             where stu_idx = %s'''
    # 접속
    conn = get_connection()
    # 쿼리 실행
    cursor = conn.cursor()
    cursor.execute(sql, (stu_idx))
    result = cursor.fetchone()
    # 데이터 추출
    result_dic = {}
    result_dic['stu_idx'] = stu_idx
    result_dic['stu_name'] = result[0]
    result_dic['stu_age'] = result[1]
    result_dic['stu_addr'] = result[2]

    conn.close()
    return result_dic

# 점수 정보를 저장한다.
def add_point(point_stu_idx, point_stu_grade, point_stu_kor) :
    # 쿼리문
    sql = '''insert into point_table
             (point_stu_idx, point_stu_grade, point_stu_kor)
             values (%s, %s, %s)'''
    # 데이터베이스 접속
    conn = get_connection()
    # 쿼리 실행
    cursor = conn.cursor()
    cursor.execute(sql, (point_stu_idx, point_stu_grade, point_stu_kor))
    conn.commit()

    conn.close()

# 학생 점수 가져오기
def get_point(stu_idx) :
    # 쿼리문
    sql = '''select point_stu_grade, point_stu_kor
             from point_table
             where point_stu_idx=%s
             order by point_stu_grade'''
    # 접속
    conn = get_connection()
    # 쿼리 실행
    cursor = conn.cursor()
    cursor.execute(sql, (stu_idx))
    result = cursor.fetchall()
    # 데이터 추출
    result_list = []

    for result_dic in result :
        temp_dic = {}
        temp_dic['point_stu_grade'] = result_dic[0]
        temp_dic['point_stu_kor'] = result_dic[1]

        result_list.append(temp_dic)

    conn.close()

    return result_list







