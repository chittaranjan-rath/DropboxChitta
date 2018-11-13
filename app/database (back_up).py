import mysql.connector
import hashlib

def get_user_details(db_cursor, id):
  query_string = "select ID, EMAIL, NAME from user where id = '"+str(id)+"'"
  db_cursor.execute(query_string)
  user_details = dict()
  result_tuples = db_cursor.fetchall()
  for t in result_tuples:
    user_details["id"] = t[0]
    user_details["email"] = str(t[1])
    user_details["name"] = str(t[2])
  return user_details

def get_all_users(db_cursor):
  query_string = "select ID, EMAIL, NAME from user"
  db_cursor.execute(query_string)
  users = list()
  result_tuples = db_cursor.fetchall()
  for t in result_tuples:
    user_detail = dict()
    user_detail["id"] = t[0]
    user_detail["email"] = str(t[1])
    user_detail["name"] = str(t[2])
    users.append(user_detail)
  return users

def get_user_id(db_cursor, email_id):
  query_string = "select ID, EMAIL from user where EMAIL='"+email_id+"'"
  db_cursor.execute(query_string)
  result_tuple = db_cursor.fetchone()
  id = result_tuple[0]
  return id

def get_used_space(db_cursor, user_id):
  query_string = "select SIZE from file where OWNER='"+str(user_id)+"'"
  db_cursor.execute(query_string)
  result_tuples = db_cursor.fetchall()
  s = 0
  for t in result_tuples:
    s+=t[0]
  return s

def create_user(db_obj, db_cursor, user_details):
  email_id = user_details["email"]
  name = user_details["name"]
  passwd = user_details["password"]
  m = hashlib.sha256()
  m.update(passwd)
  pass_hash = m.hexdigest()
  query_string = "insert into user (EMAIL,NAME,PASSWORD) values (%s, %s, %s)"
  val_string = (email_id, name, pass_hash)
  db_cursor.execute(query_string, val_string)
  db_obj.commit()

def authenticate_user(db_cursor, user_details):
  given_email_id = user_details["email"]
  given_passwd = user_details["password"]
  query_string = "select EMAIL, PASSWORD from user where EMAIL='"+given_email_id+"'"
  db_cursor.execute(query_string)
  result_tuple = db_cursor.fetchone()
  passwd_hash = result_tuple[1]
  m = hashlib.sha256()
  m.update(given_passwd)
  given_hash = m.hexdigest()
  if passwd_hash == given_hash:
    return True
  return False

def get_parent_folder_id(db_cursor, path, user_id):
  path_contents = path.split('/')
  path_contents = path_contents[1:]
  i = len(path_contents) - 1
  j = i-1
  master_parent_id = 1
  while (i >= 0) and (j >= 0):
    child_folder = path_contents[i]
    parent_folder = path_contents[j]
    query_string = "select A.ID, B.ID, A.NAME, B.NAME from folder as A join folder as B on A.PATH = B.ID where A.name='"+child_folder+"' and B.name='"+parent_folder+"' and A.owner='"+str(user_id)+"'"
    db_cursor.execute(query_string)
    result_tuples = db_cursor.fetchall()
    if len(result_tuples) > 1:
      i-=1
      j-=1
    else:
      master_parent_id = result_tuples[0][1]
      break
  curr_parent_id = master_parent_id
  i = j + 1
  while i < len(path_contents):
    query_string = "select ID from folder where NAME='"+path_contents[i]+"' and PATH='"+str(curr_parent_id)+"' and OWNER='"+str(user_id)+"'"
    db_cursor.execute(query_string)
    result_tuple = db_cursor.fetchone()
    curr_parent_id = result_tuple[0]
    i += 1
  return curr_parent_id



def create_folder(db_obj, db_cursor, folder_details):
  f_name = folder_details["name"]
  f_path = folder_details["path"]
  f_owner = folder_details["owner"]
  #parent_id = get_parent_folder_id(db_cursor, f_path, f_owner)
  query_string = "insert into folder (NAME, PATH, OWNER) values (%s, %s, %s)"
  value_tuple = (f_name, f_path, f_owner)
  db_cursor.execute(query_string, value_tuple)
  db_obj.commit()


def create_file(db_obj, db_cursor, file_details):
  f_name = file_details["name"]
  f_path = file_details["path"]
  f_size = file_details["size"]
  f_owner = file_details["owner"]
  f_permission = file_details["permission"]
  #parent_id = get_parent_folder_id(db_cursor, f_path, f_owner)
  query_string = "insert into file (NAME, PATH, SIZE, OWNER, PERMISSION) values (%s, %s, %s, %s, %s)"
  value_tuple = (f_name, f_path, f_size, f_owner, f_permission)
  db_cursor.execute(query_string, value_tuple)
  db_obj.commit()

def get_file_path(db_cursor, file_id):
  curr_file_id = file_id
  full_path = ""
  qs = "select NAME,PATH from file where ID='"+str(file_id)+"'"
  db_cursor.execute(qs)
  rt = db_cursor.fetchone()
  full_path = rt[0]
  curr_folder_id = rt[1]
  while curr_folder_id is not None:
    query_string = "select NAME,PATH from folder where ID='"+str(curr_folder_id)+"'"
    db_cursor.execute(query_string)
    result_tuple = db_cursor.fetchone()
    if result_tuple[0] == "/":
      full_path = "/"+full_path
    else:
      full_path = result_tuple[0]+"/"+full_path
    curr_folder_id = result_tuple[1]
  return full_path


def get_folder_path(db_cursor, folder_id):
  curr_folder_id = folder_id
  full_path = ""
  while curr_folder_id is not None:
    query_string = "select NAME,PATH from folder where ID='"+str(curr_folder_id)+"'"
    db_cursor.execute(query_string)
    result_tuple = db_cursor.fetchone()
    if result_tuple[0] == "/":
      full_path = "/"+full_path
    elif full_path == "":
      full_path = result_tuple[0]
    else:
      full_path = result_tuple[0]+"/"+full_path
    curr_folder_id = result_tuple[1]
  return full_path
  

def get_file_details(db_cursor, file_id):
  query_string = "select * from file where ID='"+str(file_id)+"'"
  db_cursor.execute(query_string)
  result_tuple = db_cursor.fetchone()
  file_details = dict()
  file_details["id"] = result_tuple[0]
  file_details["name"] = str(result_tuple[1])
  file_details["path"] = str(get_file_path(db_cursor, file_id))
  file_details["size"] = result_tuple[3]
  file_details["owner"] = result_tuple[4]
  file_details["permission"] = str(result_tuple[5])
  return file_details

def get_folder_details(db_cursor, folder_id):
  query_string = "select * from folder where ID='"+str(folder_id)+"'"
  db_cursor.execute(query_string)
  result_tuple = db_cursor.fetchone()
  folder_details = dict()
  folder_details["id"] = result_tuple[0]
  folder_details["name"] = str(result_tuple[1])
  folder_details["path"] = str(get_folder_path(db_cursor, folder_id))
  folder_details["owner"] = result_tuple[3]
  return folder_details

def get_folder_size(db_cursor, folder_id):
  query_string = "select SIZE from file where PATH='"+str(folder_id)+"'"
  db_cursor.execute(query_string)
  result_tuples = db_cursor.fetchall()
  s = 0
  for t in result_tuples:
    s+=t[0]
  return s

def get_folder_entries(db_cursor, folder_id):
  entries = dict()
  file_entries = list()
  query_string = "select * from file where PATH='"+str(folder_id)+"'"
  db_cursor.execute(query_string)
  result_tuples = db_cursor.fetchall()
  for result_tuple in result_tuples:
    file_details = dict()
    file_details["id"] = result_tuple[0]
    file_details["name"] = str(result_tuple[1])
    file_details["path"] = str(get_file_path(db_cursor, result_tuple[0]))
    file_details["size"] = result_tuple[3]
    file_details["owner"] = result_tuple[4]
    file_details["permission"] = str(result_tuple[5])
    file_entries.append(file_details)
  entries["files"] = file_entries
  query_string = "select * from folder where PATH='"+str(folder_id)+"'"
  db_cursor.execute(query_string)
  result_tuples = db_cursor.fetchall()
  folder_entries = list()
  for result_tuple in result_tuples:
    folder_details = dict()
    folder_details["id"] = result_tuple[0]
    folder_details["name"] = str(result_tuple[1])
    folder_details["path"] = str(get_folder_path(db_cursor, result_tuple[0]))
    folder_details["owner"] = result_tuple[3]
    folder_entries.append(folder_details)
  entries["folders"] = folder_entries
  return entries

def delete_user(db_obj, db_cursor, user_id):
  query_string = "delete from user where ID='"+str(user_id)+"'"
  db_cursor.execute(query_string)
  db_obj.commit()

def delete_file(db_obj, db_cursor, file_id):
  query_string = "delete from file where ID='"+str(file_id)+"'"
  db_cursor.execute(query_string)
  db_obj.commit()

def delete_folder(db_obj, db_cursor, folder_id):
  query_string = "delete from folder where ID="+str(folder_id)+"'"
  db_cursor.execute(query_string)
  db_obj.commit()

def move_file(db_obj, db_cursor, file_id, destination_path,user_id):
  new_folder_id = get_parent_folder_id(db_cursor,destination_path,user_id)
  query_string = "update file set PATH="+str(new_folder_id)+"' where ID='"+str(file_id)+"'"
  db_cursor.execute(query_string)
  db_obj.commit()

def move_folder(db_obj, db_cursor, folder_id, destination_path,user_id):
  new_folder_id = get_parent_folder_id(db_cursor,destination_path,user_id)
  query_string = "update folder set PATH="+str(new_folder_id)+"' where ID='"+str(folder_id)+"'"
  db_cursor.execute(query_string)
  db_obj.commit()




# mydb = mysql.connector.connect(
#   host="localhost",
#   user="ateam",
#   passwd="password",
#   database="dropbox"
# )

#mycursor = mydb.cursor()

#print get_parent_folder_id(mycursor, "/dir1/dir3/dir4", 2)
# file_details = dict()
# file_details["name"] = "file10.txt"
# file_details["path"] = "/dir1/dir3/dir4"
# file_details["size"] = 54
# file_details["owner"] = 2
# file_details["permission"] = 'public'
# create_file(mydb, mycursor, file_details)
#print str(get_folder_entries(mycursor, 2))