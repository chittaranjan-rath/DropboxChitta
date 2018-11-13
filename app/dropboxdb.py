import mysql.connector
import database as db
class DropBoxDB:
    db_obj = None
    db_cursor = None
    def __init__(self, user_name, password):
        self.db_obj = mysql.connector.connect(
        host="localhost",
        user=user_name,
        passwd=password,
        database="dropbox")
        self.db_cursor = self.db_obj.cursor()

    def get_user_details(self,id):
        return db.get_user_details(self.db_cursor, id)

    def create_user(self, user_details):
        db.create_user(self.db_obj, self.db_cursor, user_details)

    def get_all_users(self):
        return db.get_all_users(self.db_cursor)

    def get_user_id(self, email_id):
        return db.get_user_id(self.db_cursor, email_id)

    def get_used_space(self, user_id):
        return db.get_used_space(self.db_cursor, user_id)

    def authenticate_user(self, user_details):
        return db.authenticate_user(self.db_cursor, user_details)

    def modify_password(self, user_details):
        db.modify_password(self.db_obj, self.db_cursor, user_details)

    def get_root_path_id(self, user_id):
        return db.get_root_path_id(self.db_cursor, user_id)

    def get_parent_folder_id(self, path, user_id):
        return db.get_parent_folder_id(self.db_cursor, path, user_id)

    def folder_exists(self, folder_details):
        return db.folder_exists(self.db_cursor, folder_details)

    def create_folder(self, folder_details):
        db.create_folder(self.db_obj, self.db_cursor, folder_details)

    def create_file(self, file_details):
        db.create_file(self.db_obj, self.db_cursor, file_details)

    def file_exists(self, db_cursor, file_details):
        return db.file_exists(self.db_cursor, file_details)

    def search_files(self, user_id, search_string):
        return db.search_files(self.db_cursor, user_id, search_string)

    def get_file_path(self, file_id):
        return db.get_file_path(self.db_cursor, file_id)

    def get_navigation_context(self, folder_id):
        return db.get_navigation_context(self.db_cursor, folder_id)

    def get_folder_path(self, folder_id):
        return db.get_folder_path(self.db_cursor, folder_id)

    def modify_file_permission(self, file_id, new_permission):
        db.modify_file_permission(self.db_obj, self.db_cursor, file_id, new_permission)

    def get_file_details(self, file_id):
        return db.get_file_details(self.db_cursor, file_id)

    def get_folder_details(self, folder_id):
        db.get_folder_details(self.db_cursor, folder_id)

    def get_folder_size(self, folder_id):
        return db.get_folder_size(self.db_cursor, folder_id)

    def get_folder_entries(self, folder_id):
        return db.get_folder_entries(self.db_cursor, folder_id)

    def delete_user(self, user_id):
        db.delete_user(self.db_obj, self.db_cursor, user_id)

    def delete_file(self, file_id):
        db.delete_file(self.db_obj, self.db_cursor, file_id)

    def delete_folder(self, folder_id):
        db.delete_folder(self.db_obj, self.db_cursor, folder_id)

    def move_file(self, file_id, path_id):
        db.move_file(self.db_obj, self.db_cursor, file_id, path_id)

    def move_folder(self, folder_id, path_id, user_id):
        db.move_folder(self.db_obj, self.db_cursor, folder_id, path_id)