# Flask 모듈을 포함한다.
from flask import Flask, render_template

# Flask 객체를 생성한다.(매개 변수 : 아무 문자열)
# template_folder : 사용할 템플릿 html이 들어 있는 폴더를 지정한다.
app = Flask(__name__, template_folder='view')

# app.route : 클라이언트가 요청했을 때 호출될 함수를 등록
@app.route("/")
def index() :
    # 클라이언트 브라우저에게 전달할 문자열을 반환한다.
    # return 'Hello World'
    # return '<h1>Hello World</h1><br/><h3>flask</h3>'

    # 클라이언트 브라우저에게 전달할 문자열을 html 파일로 부터 읽어와
    # 생성한다.
    html = render_template('index.html')
    return html
#app.run()

# 서버 가동
# host='0.0.0.0' : 모든 디바이스에서 접속 가능하도록 설정
# port=80 : 포트 번호를 80번으로 설정
app.run(host='0.0.0.0', port=80, debug=true)

# SET FLASK_APP=main.py
# flask run
