from flask import Blueprint, render_template, request, session, redirect
from board import board_dao
import os
import time

board_blue = Blueprint('board_blue', __name__)

@board_blue.route('/board_list', defaults={'board_idx' : 1, 'page' : 1})
@board_blue.route('/board_list/<board_idx>', defaults={'page' : 1})
@board_blue.route('/board_list/<board_idx>/<page>')
def board_list(board_idx, page) :
    # print(f'board_idx : {board_idx}')

    # 게시판 이름을 가져온다.
    board_name = board_dao.get_board_name(board_idx)

    data_dic = {}
    data_dic['board_name'] = board_name
    data_dic['board_idx'] = board_idx

    data_list = board_dao.getContentList(board_idx, int(page))

    page_count, page_min, page_max, prev, next = board_dao.get_pagenation_info(board_idx, page)


    data_dic['page_count'] = page_count
    data_dic['page_min'] = page_min
    data_dic['page_max'] = page_max
    data_dic['prev'] = prev
    data_dic['next'] = next
    data_dic['now_page'] = int(page)

    html = render_template('board/board_list.html', data_dic=data_dic, data_list=data_list)
    return html

@board_blue.route('/board_write/<board_idx>/<page>')
def board_write(board_idx, page) :
    data_dic = {}
    data_dic['board_idx'] = board_idx
    data_dic['page'] = page
    html = render_template('board/board_write.html', data_dic=data_dic)
    return html

@board_blue.route('/board_read/<board_idx>/<content_idx>', defaults={'page' : 1})
@board_blue.route('/board_read/<board_idx>/<content_idx>/<page>')
def board_read(board_idx, content_idx, page) :

    # 글 정보를 가져온다.
    data_dic = board_dao.get_content(content_idx)

    data_dic['page'] = page

    html = render_template('board/board_read.html', data_dic=data_dic)
    return html

@board_blue.route('/board_modify/<content_idx>/<page>')
def board_modify(content_idx, page) :

    data_dic = board_dao.get_content(content_idx)
    data_dic['page'] = page

    html = render_template('board/board_modify.html', data_dic=data_dic)
    return html

@board_blue.route('/board_write_pro', methods=['post'])
def board_write_pro() :
    # 파라미터 데이터 추출한다.
    board_subject = request.form['board_subject']
    board_content = request.form['board_content']
    writer_idx = session['user_idx']
    board_idx = request.form['board_idx']
    # 파일 데이터를 추출한다.
    if 'board_image' in request.files :
        board_image = request.files['board_image']
        file_name = str(int(time.time())) + board_image.filename

        a1 = os.getcwd() + '/upload/' + file_name
        board_image.save(a1)

    else :
        file_name = None
    # 저장한다.
    content_idx = board_dao.add_content(board_subject, board_content, writer_idx, board_idx, file_name)

    return redirect(f'/board_read/{board_idx}/{content_idx}')

# 글 삭제 처리
@board_blue.route('/delete_content', methods=['post'])
def delete_content() :

    content_idx = request.form['content_idx']

    board_dao.delete_content(content_idx)

    return 'OK'

@board_blue.route('/modify_content_pro', methods=['post'])
def modify_content() :
    board_subject = request.form['board_subject']
    board_content = request.form['board_content']
    board_content_idx = request.form['board_content_idx']
    board_idx = request.form['board_idx']
    if 'board_image' in request.files :
        board_image = request.files['board_image']
        file_name = str(int(time.time())) + board_image.filename

        a1 = os.getcwd() + '/upload/' + file_name
        board_image.save(a1)

    else :
        file_name = None

    board_dao.modify_content(board_content_idx, board_subject, board_content, file_name)

    return redirect(f'/board_read/{board_idx}/{board_content_idx}')