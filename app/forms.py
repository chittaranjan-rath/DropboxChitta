from flask_wtf import FlaskForm
from wtforms import StringField
from wtforms.validators import DataRequired,Length,Email

class RegistrationForm(FlaskForm):
    username = StringField('Username',validators=[DataRequired(),Length(min=2, max=20)])
    email = Str
