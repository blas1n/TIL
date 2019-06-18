from flask import Blueprint, render_template

main_blue = Blueprint('main_blue', __name__)

@main_blue.route('/')
def index() :
    html = render_template('main/index.html')
    return html