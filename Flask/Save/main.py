from flask import Flask, render_template, request, make_response, session

app = Flask(__name__, template_folder='view')
# 세션 사용을 위한 secret_key
app.secret_key = 'soijowijfwpojfwoe'

# 전역변수
# 파이썬 프로그램(웹서버)가 종료될 때 까지 유지된다.
# 모든 클라이언트 요청에 대해 똑같은 값을 사용한다.
data1 = '전역변수'

@app.route('/')
def index() :
    html = render_template('index.html')
    return html

@app.route('/input')
def input() :
    # 파라미터 값 추출
    global data1
    data1 = request.args.get('data1')
    # 지역변수 : 변수가 선언된 함수 내부에서만 사용이 가능하다.
    data2 = request.args.get('data2')
    data3 = request.args.get('data3')
    data4 = request.args.get('data4')
    #세션에 저장
    # 세션 : 브라우저가 최초에 접속을 하게 되면 만들어지는 메모리 영역이다.
    # 브라우저 마다 하나씩 생성되고 브라우저를 닫거나 30분간 새로운 요청이
    # 발생하지 않으면 소멸된다. 로그인 기능 등에서 사용한다.
    session['data3'] = data3

    # 쿠키 : 사용자 브라우저에 저장된다. 브라우저가 요청할 때 쿠키에 저장된
    # 정보를 모두 읽어서 서버로 보내게 된다. 저장시에는 서버에서 응답결과를
    # 보낼 때 쿠키 저장 정보를 담아서 보내고 이를 받은 브라우저가 사용자
    # 브라우저를 통해 저장한다.
    # 응답 객체를 만든다.
    response = make_response(f'저장 완료 - 지역변수 : {data2}')
    # 쿠키 저장 정보를 셋팅한다.
    response.set_cookie('data4', data4, max_age=365 * 24 * 60 * 60)

    return response

@app.route('/output')
def output() :
    html = f'<h3>전역변수 : {data1}</h3>'
    # html += f'<h3>지역변수 : {data2}</h3>'
    # 세션에 저장되어 있는지 확인한다.
    if 'data3' in session :
        html += f'<h3>세션 : {session["data3"]}</h3>'

    # 브라우저가 요청시 전달한 쿠키 정보를 읽어온다.
    data4 = request.cookies.get('data4')
    html += f'<h3>쿠키 : {data4}'

    return html

app.run(host='0.0.0.0', port=9090)