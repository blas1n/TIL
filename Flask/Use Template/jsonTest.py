import json

from flask import Flask, render_template, request

app = Flask(__name__, template_folder='view')

@app.route('/')
def index() :

    test_dic = {
        'a1' : 100,
        'a2' : '문자열'
    }
    
    json_data = json.dumps(test_dic, indent=4, ensure_ascii=False) 
    # indent 는 들여쓰기, ensure_ascii 는 한글 처리

    print(json_data)
    
    python_data = json.loads(json_data) 
    print(python_data)
    print(type(json_data))
    print(type(python_data))
    return json_data
    
app.run(host='0.0.0.0', port=80)

