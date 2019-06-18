from flask import Flask, render_template

app = Flask(__name__, template_folder='view')

@app.route('/ajax')
def ajax():
    return 'Hello World'

@app.route('/')
def index() :
    html = render_template('index.html')
    return html

app.run()