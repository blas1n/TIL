from flask import Flask, render_template, request

app = Flask(__name__, template_folder='view')

@app.route('/')
def index() :
    html = render_template('index.html')
    return html

@app.route('/test1')
def test1() :
    return 'test1'

@app.route('/test1/sub1')
def test1_sub1() :
    return 'test1 sub1'

@app.route('/test1/sub2')
def test1_sub2() :
    return 'test1 sub2'

@app.route('/test2/<data1>')
def test2_data1(data1) :
    return f'test2 data1 : {data1}'

@app.route('/test2/<data1>/<data2>')
def test2_data1_data2(data1, data2) :
    return f'test2 data1 : {data1}, data2 : {data2}'

@app.route('/test3')
@app.route('/test4')
def test3_or_test4() :
    return f'test3 or test4'

@app.route('/test5', defaults={'data1' : 1, 'data2' : 2})
@app.route('/test5/<data1>', defaults={'data2' : 20})
@app.route('/test5/<data1>/<data2>')
def test5(data1, data2) :
    return f'test5 data1 : {data1}, data2 : {data2}'

'''
@app.route('/user/<username>')
def show_user_profile(username):\
    # show the user profile for that user
    return 'User %s' % username
     
@app.route('/user/<username>/<int:age>')
def show_user_profile_age(username, age):
    return 'Username %s, 나이 %d' % (username, age) 
'''

app.run()

