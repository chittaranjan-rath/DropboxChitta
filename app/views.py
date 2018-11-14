import os
from flask import render_template,request,flash,redirect,url_for,session,Response,send_from_directory,send_file
from app import app 
from dropboxdb import DropBoxDB
from flask import Flask, render_template, stream_with_context, request, Response, session, Session, json
import copy
from jinja2 import Environment, PackageLoader, select_autoescape
import time
from dropboxdb import DropBoxDB
import thread
import threading
#DropBoxDB
#import dropboxdb
#127.0.0.1 localhost
lock=thread.allocate_lock()
db_obj = DropBoxDB("praveen","S@gem0de")
APP_ROOT = os.path.dirname(os.path.abspath(__file__))
@app.before_request
def require_login():
    allowed_routes = ['login' , 'register']
    if request.endpoint not in allowed_routes and 'email' not in session:
        return redirect(url_for('login'))


@app.route('/delete_file/<id>/')
def delete_file(id = None):
    print("In delete file{0}".format(id))
    db_obj.delete_file(id)
    return

@app.route('/delete_folder/<id>/')
def delete_folder(id = None):
    print("In delete folder {0}".format(id))
    db_obj.delete_folder(id)
    return

@app.route('/move_file/', methods = ['GET', 'POST'])
def move():
    if(request.method == "POST"):
        print("In move file")
        src_id = request.form["src_id"]
        dest_id = request.form["dest_id"]
        print("In move folder src:{0} dest{1}".format(src_id,dest_id))
        db_obj.move_file(src_id,dest_id)
        return

@app.route('/get_folder_list/<id>/', methods = ['GET', 'POST'])
def get_folder_entries(id = None):
    print("In get folder list {0}".format(id))
    files = db_obj.get_folder_entries(id)
    return files["folders"]

@app.route('/get_nav_context/<id>/', methods = ['GET', 'POST'])
def get_nav_context(id = None):
    print("In get nav list {0}".format(id))
    lock.acquire()
    files = db_obj.get_navigation_context(int(id))
    lock.release()
    return json.dumps(files)



@app.route('/view/')
@app.route('/view/<id>/')
def view(id=None):
    session['id'] = 1
    if(id == None):
        lock.acquire()
        files = db_obj.get_folder_entries(db_obj.get_root_path_id(session["id"]))
        lock.release()
        # print(files)
        return Response(render_template('homePage.html', data=files))
    else:
        lock.acquire()
        files = db_obj.get_folder_entries(int(id))
        lock.release()
        # print(files)
        return json.dumps(files)


@app.route("/homePage/")
def homePage():
    return render_template('homePage.html')

# @app.route("/")  # home/index route
# @app.route("/index",  methods=['GET','POST'])
# def index():
#     error=''
#     try:
#         if(request.method == "POST"):
#             sub = request.form["submit"]
            
#             flash(sub)
           
#             if(sub):
#                 return redirect(url_for('upload'))
#             else:
#                 error = "Invalid try again"
#                 return render_template('login.html',title='login',error=error)
#     except Exception as e:
#         flash(e)
#         return render_template('login.html',title='login',error=error)
#     return render_template('homePage.html',title="homepage")

@app.route('/download/', methods = ['GET', 'POST'])
def download():
    if(request.method == "POST"):
    #if(True):
        print("In dload file")
        f_name = request.form["f_name"]
        dest_path = request.form["dest_path"]
        #f_name = request.args.get('f_name')
        dest_path="#"
        print("please dload ",f_name," from ",dest_path)
        print("app root is ",APP_ROOT)
        target = "/".join([APP_ROOT,"uploaded"])
        
        if(dest_path=="#" or dest_path is None):
            print(target)
            print("inside #")
            target = "/".join([target,session['email']])
            final_path = "/".join([target,f_name])
            
            print "For download: ",final_path
            #return final_path
            try:
                return send_file(final_path , as_attachment=True)
            except Exception as e:
                print e 
            #send_from_directory(directory=target, filename=f_name)
        #print("In move folder src:{0} dest{1}".format(src_id,dest_id))
    return "dummy val"

@app.route("/upload", methods=['GET','POST'])
def upload():
    print("app root is ",APP_ROOT)
    target = "/".join([APP_ROOT,"uploaded"])
    #target = APP_ROOT
    target = "/".join([target,session['email']])
    #path here
    print("In python recieved path = " + request.form["pathID"])
    print("In python recieved path = " + db_obj.get_folder_path(request.form["pathID"]))
    full_path = db_obj.get_folder_path(request.form["pathID"])

    print(os.path.isdir(session['email']))
    if (not os.path.isdir(target)):
        os.mkdir(target)
    for file in request.files.getlist("file"):
        print(file)
        filename = file.filename
        destn = "/".join([target,filename])
        file.save(destn)
        file_length = os.stat(destn).st_size
        print("file len is",file_length)
        file_details = dict()
        file_details["name"] = filename
        file_details["path"] = 5
        file_details["size"] = file_length
        file_details["owner"] = 1
        file_details["permission"] = "private"
        db_obj.create_file(file_details)
        return redirect(url_for('view'))
        #return view()
    return render_template('upload.html')

@app.route("/about")
def about():
    return render_template('about.html')


@app.route("/changepassword/",methods=['GET','POST'])
def changepassword():
    user_details = dict()
    user_details["email"] = session['email']
    
    user_details["old_password"] = request.form['oldpassword']
    print user_details["old_password"]
    user_details["new_password"] = request.form['newpassword']
    
    db_obj.modify_password(user_details)
    print("password changed successfully")
    return "Dsadsa"


@app.route("/createFolder/",methods=['GET','POST'])
def createFolder():
    folder_details = dict()
    folder_details['name'] = request.form['foldername']
    print folder_details['name']
    folder_details["path"] = request.form['pathID']
    print("In create folder, path = {0}".format(folder_details["path"]))
    owner_id = db_obj.get_user_id(session['email'])
    print(owner_id)
    folder_details["owner"] = owner_id
    db_obj.create_folder(folder_details)
    print("succesfully created folder")
    return "ousdaiudsy"

@app.route("/register", methods=['GET','POST'])
def register():
    error=''
    try:
        if(request.method == "POST"):
            print("in register")
            print("in register 1")           
            to_register_email = request.form["email"]
            print("in register 2")
            print(to_register_email)
            to_register_pwd = request.form['password']
            print("in register 3")
            print(to_register_pwd)
            to_register_name = request.form['name']
            print("in register 4")
            user_details = dict()
           
            
            print(to_register_name)
            user_details["email"] = to_register_email
            user_details["name"] = to_register_name
            user_details["password"] = to_register_pwd
            db_obj.create_user(user_details)
            flash(to_register_email)
            flash(to_register_name)
            

    except Exception as e:
        flash(e)
        return render_template('register.html',title='register',error=error)

    #return render_template('login.html',title='login',error=error)
    
    return render_template('register.html',title='register',error=error)
    


@app.route("/login",methods=['GET','POST'])
def login():
    error=''
    try:
        if(request.method == "POST"):
            attempted_email = request.form["email"]
            attempted_pwd = request.form['password']
            flash(attempted_email)
            flash(attempted_pwd)
            
            #user_details = dict()
            #user_details["email"] = "ab@gmail.com"
            #user_details["name"] = "ab"
            #user_details["password"] = "ab"
            #db_obj.create_user(user_details)

          


            user_details = dict()
            user_details["email"] = attempted_email
            #user_details["name"] = "Tarun"
            user_details["password"] = attempted_pwd
            db_obj.authenticate_user(user_details)        

            #if(attempted_email=="chitta.vssut@gmail.com" and attempted_pwd=="123"):
            if(db_obj.authenticate_user(user_details)):
                session['email'] = attempted_email
                return redirect(url_for('view'))
            else:
                error = "Invalid try again"
                return render_template('login.html',title='login',error=error)
    except Exception as e:
        flash(e)
        return render_template('login.html',title='login',error=error)

    return render_template('login.html',title='login',error=error)
    
@app.route("/logout")
def logout():
    del session['email']
    return redirect(url_for('login'))