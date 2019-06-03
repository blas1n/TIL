from flask import Flask, render_template, request

app = Flask(__name__, template_folder='view')


@app.route("/")
def index() :
    html = render_template('index.html')
    return html

# get : 파라미터 데이터를 주소에 붙혀서 보낸다. 주소에 직접 붙혀서 보내기
#       때문에 전체 용량이 적다. 이 때문에 속도가 빠르다.
#       영문,숫자,특수문자 일부만 가능하며 전체 주소가 255글자 이상을
#       넘지 못한다는 단점을 가지고 있다.
# post : 파라미터 데이터를 요청 정보 내부에 숨겨서 보낸다.
#        부가 정보가 추가로 들어가기 때문에 전체 용량이 get보다 많으며
#        이 때문에 속도가 느리다.
#        모든 문자를 전달할 수 있으며 길이에 제한이 없다.
# 사용자에게 입력받는 데이터 => post, 그 외 => get

@app.route('/second', methods=['GET', 'POST'])
def second() :
    data3 = request.values.get('data2')
    # 요청 방식으로 분기한다.
    if request.method == 'POST' :
        # 파라미터 데이터를 추출한다.
        data1 = request.form['data1']
        data2 = request.form.get('data2')
        return f'data1 : {data1}, data2 : {data2}, data3 : {data3}'
    elif request.method == 'GET' :
        # 파라미터 데이터를 추출한다.
        data1 = request.args['data1']
        data2 = request.args.get('data2')
        return f'data1 : {data1}, data2 : {data2}, data3 : {data3}'
    
    return 'Hello World'


app.run(host='0.0.0.0', port=80)