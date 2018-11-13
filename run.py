from app import app

if __name__ =='__main__':
    app.config['SECRET_KEY'] = 'redsfsfsfsfis'
    app.run(debug=True)