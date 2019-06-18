import pymysql

# 데이터 베이스에 접속하는 함수
def get_connection() :
    conn = pymysql.connect(host='127.0.0.1', user='root',
            password='1234', db='minidb', charset='utf8')

    return conn

# 사용자 정보를 저장하는 함수
def add_user(user_name, user_id, user_pw) :
    # 쿼리문
    sql = '''insert into user_table 
             (user_name, user_id, user_pw) values (%s, %s, %s)'''

    # 접속
    conn = get_connection()
    # 쿼리실행
    cursor = conn.cursor()
    cursor.execute(sql, (user_name, user_id, user_pw))

    # 접속해제
    conn.commit()
    conn.close()

# 아이디가 있는지 확인하는 함수
def check_user_id(input_id) :
    # 쿼리문
    sql = 'select * from user_table where user_id = %s'
    # 접속
    conn = get_connection()
    # 쿼리 실행
    cursor = conn.cursor()
    cursor.execute(sql, (input_id))
    # 데이터를 가져온다.
    result = cursor.fetchone()

    if result == None :
        conn.close()
        return 'YES'
    else :
        conn.close()
        return 'NO'

# 아이디 비밀번호 확인
def check_login(user_id, user_pw) :
    # 쿼리문
    sql = '''select user_idx, user_name
             from user_table
             where user_id=%s and user_pw=%s'''
    # 접속
    conn = get_connection()
    # 쿼리 실행
    cursor = conn.cursor()
    cursor.execute(sql, (user_id, user_pw))
    # 데이터를 가져온다.
    result = cursor.fetchone()

    if result == None :
        conn.close()
        return 'NO'
    else :
        user_idx = result[0]
        conn.close()
        return user_idx













