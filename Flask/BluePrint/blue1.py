from flask import Blueprint, render_template, request

blue100 = Blueprint('blue1', __name__, template_folder='view/sub1')

@blue100.route('/test3')
def test3() :
    html = render_template('test2.html')
    return html