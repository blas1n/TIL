from flask import Blueprint, render_template

blue200 = Blueprint('blue200', __name__)

@blue200.route('/test4')
def test4() :
    html = render_template('sub2/test4.html')
    return html

