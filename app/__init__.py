from flask import Flask #import Flask class
app = Flask(__name__)  # set app variable to instance of Flask class


# Import Views from the app module. (DO NOT Confuse with app variable)
from app import views

# Import is at the end to avoid circular reference