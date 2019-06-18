from flask import Flask, render_template, request, session, redirect, url_for

app = Flask(__name__, template_folder='view')
app.secret_key = 'soijowijfwpojfwoe'

@app.route('/')
@app.route('/index')
def index():
	if not session.get('logged_in'):
		return render_template('index.html')
	else:
		if request.method == 'POST':
			username = getname(request.form['username'])
			return render_template('index.html', data=getfollowedby(username))
		return render_template('index.html')


@app.route('/login', methods=['GET', 'POST'])
def login():
	if request.method == 'GET':
		return render_template('login.html')
	else:
		name = request.form['username']
		passw = request.form['password']
		try:
			#DB에서 회원정보 조회
			session['logged_in'] = True
			session['user_id'] = name
			return redirect(url_for('index'))
		except:
			return "Dont Login"


@app.route('/register/', methods=['GET', 'POST'])
def register():
	if request.method == 'POST':
		username=request.form['username']
		password=request.form['password']

		#DB에 회원정보 등록		
		return render_template('login.html')
	return render_template('register.html')


@app.route('/logout')
def logout():
	if session.get('logged_in'):
		del session['logged_in']
	if session.get('user_id'):
		del session['user_id']		

	session.pop('logged_in', None)
	session.pop('user_id', None)
	return redirect(url_for('index'))

@app.route('/about')
def about():
  	return render_template('about.html', title='About')

app.run(host='0.0.0.0', port=80)

