import pymysql

#######ex. 1
# Open database connection
db = pymysql.connect('127.0.0.1', 'actor', 'actor', 'movies') #change according to your settings

# prepare a cursor object using cursor() method
cursor = db.cursor()

# execute SQL query using execute() method.
cursor.execute("SELECT VERSION()")

# Fetch a single row using fetchone() method.
data = cursor.fetchone()
print ('Database version : %s ' % data)
print('\nExample Query #1')
sqlQuery = '''select g1, g2, count(dif_directors1) as dif_directors 
from (select g1.genre_id as g1, g2.genre_id as g2, mhd1.director_id as dif_directors1
    from   genre g1, genre g2, movie m1, movie m2, movie_has_director mhd1, movie_has_director mhd2, movie_has_genre mhg1, movie_has_genre mhg2, director d
    where  g1.genre_id < g2.genre_id
    and    g1.genre_id = mhg1.genre_id
    and    g2.genre_id = mhg2.genre_id
    and    mhg1.genre_id <> mhg2.genre_id
    and    mhg1.movie_id = m1.movie_id
    and    mhg2.movie_id = m2.movie_id
    and    m1.movie_id <> m2.movie_id
    and    d.director_id = mhd1.director_id
    and    d.director_id = mhd2.director_id
    and    mhd1.movie_id = m1.movie_id
    and    mhd2.movie_id = m2.movie_id
    group by g1.genre_id, g2.genre_id, mhd1.director_id
      union
    select g1.genre_id as g1, g2.genre_id as g2,  mhd.director_id as dif_directors2
    from   genre g1, genre g2, movie m, movie_has_director mhd, movie_has_genre mhg1, movie_has_genre mhg2, director d
    where  g1.genre_id < g2.genre_id
    and    g1.genre_id = mhg1.genre_id
    and    g2.genre_id = mhg2.genre_id
    and    mhg1.movie_id = m.movie_id
    and    mhg2.movie_id = m.movie_id
    and    mhg1.movie_id = mhg2.movie_id
    and    d.director_id = mhd.director_id
    and    mhd.movie_id = m.movie_id
    group by g1.genre_id, g2.genre_id,mhd.director_id ) as res
group by g1, g2;'''

print('''Βρείτε τους τίτλους των ταινιών που παίζει ηθοποιος με επώνυμο “Allen” και το
είδος της ταινίας είναι “Comedy”\n''')
cursor.execute(sqlQuery)
'''rows = cursor.fetchall()
for row in rows:
    if row[2] > 5:
        print("%d %d %d " % row)
        print(row)'''

for i in range(5):
    fet = cursor.fetchone()
    print(fet)
         #thelei format gia olo to table allios vgazei error afou exei 3 stixoia theloume 3 % prosdioristika SYMVATA me to kathe collumn
# edo as poume theloume apto 3o item tou kathe row pou einai to count diaforetikon skinotheton theloume mono autes pou einai megalyteres apo 5
#elenxos an ola pigan kala meta apo tropopoihsh ths DB apto query, to rollback = revert_changes
'''
try:
   # Execute the SQL command
   cursor.execute(sql)
   # Commit your changes in the database
   db.commit()
except:
   # Rollback in case there is any error
   db.rollback()
'''


# disconnect the Database connection
db.close()

