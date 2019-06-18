from flask import Flask, render_template, request, Blueprint
from blue1 import blue100
from blue2 import blue200

app = Flask(__name__, template_folder='view')
# blue print 등록
app.register_blueprint(blue100)
app.register_blueprint(blue200)

@app.route('/test1')
def test1() :
    html = render_template('sub1/test1.html')
    return html

@app.route('/test2')
def test2() :
    html = render_template('sub2/test3.html')
    return html

app.run()