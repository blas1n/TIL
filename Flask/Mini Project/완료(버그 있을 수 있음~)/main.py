from flask import Flask
from main import index
from board import board_list
from user import user

app = Flask(__name__, template_folder='view', static_url_path='', static_folder='upload')
# 세션 사용을 위한 secret_key
app.secret_key = 'selkjfoiwejfoiwjfoijiofj'

app.register_blueprint(index.main_blue)
app.register_blueprint(board_list.board_blue)
app.register_blueprint(user.user_blue)

app.run(host='0.0.0.0', port=80)