import pymysql

# 데이터 베이스에 접속하는 함수
def get_connection() :
    conn = pymysql.connect(host='127.0.0.1', user='root',
            password='1234', db='minidb', charset='utf8')

    return conn

# 게시판 이름을 가져오는 함수
def get_board_name(board_idx) :
    # 쿼리문
    sql = 'select board_name from board_table where board_idx=%s'
    # 접속
    conn = get_connection()
    # 쿼리 실행
    cursor = conn.cursor()
    cursor.execute(sql, (board_idx))
    # 데이터 추출한다.
    result = cursor.fetchone()
    board_name = result[0];

    conn.close()
    return board_name

# 게시글 추가 함수
def add_content(content_subject, content_text, content_writer_idx, content_board_idx, file_name) :
    # 쿼리문
    sql = '''insert into content_table
             (content_subject, content_text, content_date,
              content_writer_idx, content_board_idx, content_status, content_file)
             values (%s, %s, sysdate(), %s, %s, 1, %s)'''
    # 접속
    conn = get_connection()
    # 쿼리 실행
    cursor = conn.cursor()

    cursor.execute(sql, (content_subject, content_text,
                         content_writer_idx, content_board_idx, file_name))
    conn.commit()

    # 글 번호를 가져오는 쿼리문
    sql2 = '''select max(content_idx) 
              from content_table
              where content_status=1'''
    cursor.execute(sql2)
    result2 = cursor.fetchone()
    content_idx = result2[0]

    conn.close()
    return content_idx

# 게시글 정보를 반환하는 함수
def get_content(content_idx) :
    # 쿼리문
    sql = '''select a2.user_name, a1.content_subject,
                    a1.content_text, a1.content_file, 
                    a1.content_board_idx, a1.content_writer_idx
             from content_table a1, user_table a2
             where a1.content_writer_idx = a2.user_idx
                   and a1.content_idx=%s'''
    # 접속
    conn = get_connection()
    # 쿼리 실행
    cursor = conn.cursor()
    cursor.execute(sql, (content_idx))
    # 데이터를 가져온다.
    result = cursor.fetchone()
    data_dic = {}
    data_dic['user_name'] = result[0]
    data_dic['content_subject'] = result[1]
    data_dic['content_text'] = result[2]
    data_dic['content_file'] = result[3]
    data_dic['content_idx'] = content_idx
    data_dic['content_board_idx'] = result[4]
    data_dic['content_writer_idx'] = result[5]

    conn.close()

    return data_dic

# 게시글 목록 가져오는 함수
def getContentList(board_idx, page) :

    # 한페이지에 10개씩..
    start = (page -1) * 10

    sql = '''select a1.content_idx, a1.content_subject, a2.user_name, a1.content_date
             from content_table a1, user_table a2
             where a1.content_writer_idx = a2.user_idx
                   and content_board_idx=%s and content_status=1
             order by content_idx desc
             limit %s, 10'''

    conn = get_connection()
    cursor = conn.cursor()
    cursor.execute(sql, (board_idx, start))

    result_list = cursor.fetchall()

    data_list = []

    for row in result_list :
        temp_dic = {}
        temp_dic['content_idx'] = row[0]
        temp_dic['content_subject'] = row[1]
        temp_dic['content_writer_name'] = row[2]
        temp_dic['content_date'] = row[3]
        temp_dic['board_idx'] = board_idx

        data_list.append(temp_dic)

    conn.close()

    return data_list

# 하단 부 pagenation 구성을 위한 함수
def get_pagenation_info(board_idx, page) :
    # 글 전체의 개수를 가져온다.
    sql = '''select count(*) from content_table 
             where content_board_idx=%s and content_status=1'''

    conn = get_connection()
    cursor = conn.cursor()
    cursor.execute(sql, (board_idx))
    result = cursor.fetchone()

    count = result[0]
    # 전체 페이지 개수
    page_count = count // 10
    if page_count % 10 > 0 :
        page_count += 1


    # pagenation 최소 값
    min = ((int(page) - 1) // 10) * 10 + 1
    max = min + 9

    if max > page_count :
        max = page_count

    prev = min - 1
    next = max + 1

    print(page_count, max)

    return page_count, min, max, prev, next

# 글 삭제 처리
def delete_content(content_idx) :

    sql = 'update content_table set content_status=0 where content_idx=%s'

    conn = get_connection()
    cursor = conn.cursor()
    cursor.execute(sql, (content_idx))

    conn.commit()
    conn.close()

# 글 수정 처리
def modify_content(content_idx, content_subject, content_text, file_name) :

    sql = 'update content_table set content_subject=%s, content_text=%s '

    if file_name != None :
        sql += ', content_file=%s '

    sql += 'where content_idx=%s'

    conn = get_connection()

    cursor = conn.cursor()

    if file_name != None :
        cursor.execute(sql, (content_subject, content_text, file_name, content_idx))
    else :
        cursor.execute(sql, (content_subject, content_text, content_idx))

    conn.commit()
    conn.close()





