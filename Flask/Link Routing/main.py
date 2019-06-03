from flask import Flask, render_template

# Flask 객체를 생성한다.
app = Flask(__name__, template_folder='view')

# 주소만 입력했을 경우 호출되는 함수
@app.route("/")
def index() :
    html = render_template('index.html')
    return html

# second를 요청했을 때 호출되는 함수
@app.route('/second')
def second() :
    html = render_template('second.html')
    return html

# third를 요청했을 때 호출되는 함수
@app.route('/third')
def third() :
    html = render_template('third.html')
    return html

# 서버 가동
app.run(host='0.0.0.0', port=80)

