# flask 모듈을 포함시킨다.
from flask import Flask, render_template, request

# flask 서버 객체를 생성한다.
app = Flask(__name__, template_folder='view')

# 주소만 입력했을 경우 호출되는 함수
@app.route("/")
def index() :

    # index.html 파일에서 html 데이터를 읽어온다.
    html = render_template('index.html')
    return html

# 주소 뒤에 second라고 입력했을 경우 호출되는 함수
# 요청 방식 : get, post 등등
# post : form 태그의 method가 post인 경우
# get : 주소를 직접 입력했을 때, a 태그를 클릭 했을 때
#       form 태그의 method가 없거나 get인 경우

@app.route("/second", methods=['GET', 'POST'])
def second() :
    # 요청 방식
    print(f'요청방식 : {request.method}')
    # if request.method == 'POST':
    html = render_template('second.html')
    return html

# 서버 가동
app.run(host='0.0.0.0', port=80)

