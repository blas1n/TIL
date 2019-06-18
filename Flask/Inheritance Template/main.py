from flask import Flask, render_template, request

app = Flask(__name__, template_folder='view')

@app.route('/')
@app.route('/index')
def index():
    return render_template('index.html')
    #return render_template('index.html', posts=posts)

@app.route('/about')
def about():
  return render_template('about.html', title='About')

app.run(host='0.0.0.0', port=80)

