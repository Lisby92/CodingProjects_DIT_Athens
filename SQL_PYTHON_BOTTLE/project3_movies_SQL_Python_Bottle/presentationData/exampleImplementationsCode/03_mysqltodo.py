import sys, os
import pymysql

# Open database connection
db = pymysql.connect('127.0.0.1', 'root', '', 'testdb')
# prepare a cursor object using cursor() method
cursor = db.cursor()
cursor.execute("DROP TABLE IF EXISTS todo")
sql = """CREATE TABLE todo (
   task char(100) NOT NULL,
   status bool NOT NULL 
)"""

cursor.execute(sql)

cursor.execute("INSERT INTO todo (task,status) VALUES ('Read A-byte-of-python to get a good introduction into Python',0)")
cursor.execute("INSERT INTO todo (task,status) VALUES ('Visit the Python website',1)")
cursor.execute("INSERT INTO todo (task,status) VALUES ('Test various editors for and check the syntax highlighting',1)")
cursor.execute("INSERT INTO todo (task,status) VALUES ('Choose your favorite WSGI-Framework',0)")

db.commit()


