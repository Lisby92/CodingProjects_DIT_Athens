# ----- CONFIGURE YOUR EDITOR TO USE 4 SPACES PER TAB ----- #
# ----- OPTIONAL : CONVERT IDENTATION TO TABS ------------- #
import pymysql as db
import settings
import sys

def connection():
	#=========================================================================================#
	''' use this function to create your connections '''
	#=========================================================================================#
	con = db.connect(
		settings.mysql_host, 
		settings.mysql_user, 
		settings.mysql_passwd, 
		settings.mysql_schema)
	
	return con

def updateRank(rank1, rank2, movieTitle):
	#========================================================================================#
	''' contract below :
	1. updateRank: Η συνάρτηση αυτή παίρνει από το χρήστη ως όρισμα δυο αριθμητικές τιμές
	(Rank1 & Rank2 -double από 0 μέχρι και 10) και το όνομα μίας ταινίας (Movie title)
	και ενημερώνει το πεδίο rank της ταινίας αυτής με το μέσο όρο που προκύπτει από την
	τρέχουσα τιμή της στο πεδίο αυτό (αν υπάρχει) και των δύο τιμών που έδωσε ο χρήστης.
	Αν η τιμή rank απουσιάζει τότε η τιμή θα ενημερώνεται με τον μέσο όρο τον δύο τιμών
	που έδωσε ο χρήστης. Σε περίπτωση επιτυχίας, επιστρέφει “ok”. Αντίθετα, αν δεν
	υπάρχει ταινία με αυτό το όνομα, ή αν υπάρχουν περισσότερες της μιας ταινίες με το
	ίδιο όνομα, ή αν οι αριθμητικές τιμές που εισάγει ο χρήστης είναι εκτός ορίων, επιστρέφει
	“error”.
	'''
	#========================================================================================#
	# create a new connection
	con=connection()
	# create a cursor on the connection
	cur=con.cursor()
	# initial checks, only floats in range [0,10] accepted as input
	try:
		r1 = float(rank1)
		if (r1 < 0) or (r1 > 10):
			return [("Status",),("Error : Input value out of range : [0,10]",),]
	except ValueError:
		return [("Status",),("Error : Invalid input value!",),]
	try:
		r2 = float(rank2)
		if (r2 < 0) or (r2 > 10):
			return [("Status",),("Error : Input value out of range : [0,10]",),]
	except ValueError:
		return [("Status",),("Error : Invalid input value!",),]	
	# define the query to the database
	sql_query = '''
				SELECT 	m.title, m.rank
				FROM	movie m;
				'''
	# execute the query							
	try:	
		cur.execute(sql_query)
		# fetch the data returned from the query
		data = cur.fetchall()
		# initialise the flag to indicate if input movie was found
		flag = 0
		# find the movie and get its title and rank
		for row in data:
			if row[0] == movieTitle:
				flag = 1
				title = row[0]
				rank = row[1] 
	except:
		return [("Status",),("Error : A problem occured while fetching data!",),]
	# movie not found in database
	if flag == 0:
		return [("Status",),("Sorry, movie wasn't found!",),]
	# else movie was found in database
	# case 1 : rank = NULL
	if rank == None:
		new_rank = (r1 + r2)/2
	# case 2 : rank = float	
	else:
		new_rank = (r1 + r2 + rank)/3
	# define the new query to update the movie's rank
	sql_query = '''
				UPDATE 	movie m
				SET 	rank = %f
				WHERE 	title = '%s';
				''' % (new_rank, title)
				# careful declare '%s' or else it can't find the movie
				# %f = new_rank, '%s' = title
	# execute the query			
	try:
		cur.execute(sql_query)
		# commit changes to the database
		con.commit()
	except:
		con.rollback()
		return [("Status",),("Error : A problem occured while executing the query!",),]
		# rollback in case there is any error
	# changes commited successfully , inform user
	return [("Status",),("Rank of \""+title+"\" updated successfully!",),]

def colleaguesOfColleagues(actorId1, actorId2):
	#========================================================================================#
	''' contract below :
	2. colleaguesOfColleagues: Η συνάρτηση αυτή παίρνει ως όρισμα ένα ζευγάρι (a,b) από
	κωδικούς ηθοποιών (Actor Id 1, Actor Id 2). Βρίσκει τις ταινίες που παίζουν μαζί δύο
	άλλοι ηθοποιοί (c,d) με την προϋπόθεση ότι ο c έχει παίξει σε ταινία μαζί με τον a και ο
	d έχει παίξει σε ταινία μαζί με τον b. Επιστρέφει τα εξής:
	○ Τον τίτλο της κάθε ταινίας των (c,d).
	○ Τον κωδικό του ηθοποιού c.
	○ Τον κωδικό του ηθοποιού d.
	○ Τον κωδικό του ηθοποιού a.
	○ Τον κωδικό του ηθοποιού b.
	'''
	#========================================================================================#
	# create a new connection
	con=connection()
	# create a cursor on the connection
	cur=con.cursor()
	# initialise the results table, first row = header
	table = [("movieTitle", "colleagueOfActor1", "colleagueOfActor2", "actor1","actor2",),]
	# initial checks, only positive integers accepted as input
	try:
		id1 = int(actorId1)
		id2 = int(actorId2)
		if (id1 < 0) or (id2 < 0):
			return [("Status",),("Error : Ids must be positive integers!",),]
		# treaty : will not handle same input for the IDs -> not much logic into it and data fetched will be loads 	
		elif id1 == id2:
			return [("Status",),("Error : Ids must be different!",),]	
	except ValueError:
		return [("Status",),("Error : Ids must be integers!",),]	
	# define the query to the database
	sql_query = '''
				SELECT  DISTINCT m.title, resu.*
				FROM    (SELECT c.actor_id AS c_id, d.actor_id AS d_id, a.actor_id AS a_id, b.actor_id AS b_id
						 FROM   movie m1, role r1, role r2, actor c, actor a, role r3, role r4, movie m2, actor b, actor d
						 WHERE  a.actor_id = %d
						 AND	b.actor_id = %d
						 AND	m1.movie_id = r1.movie_id
						 AND	r1.actor_id = a.actor_id
						 AND	r2.actor_id = c.actor_id
						 AND	m1.movie_id = r2.movie_id
						 AND	r1.movie_id = r2.movie_id
						 AND	r1.actor_id <> r2.actor_id
						 AND	m2.movie_id = r3.movie_id
						 AND	r3.actor_id = b.actor_id
						 AND	r4.actor_id = d.actor_id
						 AND	m2.movie_id = r4.movie_id
						 AND	r3.movie_id = r4.movie_id
						 AND	r3.actor_id <> r4.actor_id) AS resu, movie m, role c1, role c2
				WHERE	 m.movie_id = c1.movie_id
				AND		 m.movie_id = c2.movie_id
				AND		 resu.c_id = c1.actor_id
				AND		 resu.d_id = c2.actor_id
				AND		 resu.c_id <> resu.d_id
				AND		 resu.a_id <> resu.d_id
				AND		 resu.b_id <> resu.c_id
				GROUP BY m.title, resu.c_id, resu.d_id, resu.a_id, resu.b_id;
				''' % (id1, id2)
	# execute the query
	try:
		cur.execute(sql_query)
		# get the data fetched
		data = cur.fetchall()
		# add them to the results table
		table.extend(data)
	# something went wrong throw exception	
	except:
		return [("Status",),("Error : A problem occured while executing the query!",),]
	# return the results table to the website caller
	return table

def actorPairs(actorId):
	#========================================================================================#
	''' contract below :
	3. actorPairs: Η συνάρτηση αυτή δέχεται ως όρισμα τον κωδικό ενός ηθοποιού.
	Βρίσκει τους ηθοποιούς ο καθένας από τους οποίους έχει παίξει μόνο σε διαφορετικά
	είδη ταινιών από αυτόν αλλά που μαζί μ’ αυτόν έχει παίξει σε τουλάχιστον επτά
	διαφορετικά είδη. Επιστρέφει το εξής:
	○ Τους κωδικούς των ηθοποιών που πληρούν την παραπάνω συνθήκη.
	'''
	#========================================================================================#
	# create a new connection
	con=connection()
	# create a cursor on the connection
	cur=con.cursor()
	# initialise the results table
	table = [("actor2Id",),]
	# initial checks, only positive integers accepted as input
	try:
		inp_id = int(actorId)
		if inp_id < 0:
			return [("Status",),("Error : Id must be a positive integer!",),]
	except ValueError:
		return [("Status",),("Error : Id must be an integer!",),]
	# define the query to the database	
	sql_query = '''
				SELECT  DISTINCT g1.genre_id
				FROM	genre g1, movie m1, movie_has_genre mhg1, actor a1, role r1
				WHERE	a1.actor_id = %d
				AND		g1.genre_id = mhg1.genre_id
				AND		mhg1.movie_id = m1.movie_id
				AND		a1.actor_id = r1.actor_id
				AND		r1.movie_id = m1.movie_id;
				''' % (inp_id)
	# execute the query
	try:
		inp_dif_genres = cur.execute(sql_query)
		# will be used as counter for the next part
		if inp_dif_genres == 0:
			# return empty table, input actor not in database,
			# or he/she is but, hasn't played yet in any of the database's movies
			return table
		print(inp_dif_genres)	
		# else, get list of dif_genres for the input actor
		data = cur.fetchall()
		# define the next query to the database
		sql_query = '''
					SELECT 	   a.actor_id, COUNT(DISTINCT mhg.genre_id) AS dif_gens
					FROM	   (SELECT 		a0.actor_id
								FROM 		actor a0
								WHERE 		a0.actor_id NOT IN(SELECT DISTINCT	a1.actor_id
															   FROM				(SELECT DISTINCT  		g2.genre_id
																				 FROM					genre g2, movie m2, movie_has_genre mhg2, actor a2, role r2
																				 WHERE					a2.actor_id = %d
																				 AND					g2.genre_id = mhg2.genre_id
																				 AND					mhg2.movie_id = m2.movie_id
																				 AND					a2.actor_id = r2.actor_id
																				 AND					r2.movie_id = m2.movie_id) AS g1, movie m1, role r1, actor a1, movie_has_genre mhg1
																WHERE				a1.actor_id = r1.actor_id
																AND					r1.movie_id = m1.movie_id
																AND					g1.genre_id = mhg1.genre_id
																AND					mhg1.movie_id = m1.movie_id
																AND					a1.actor_id <> %d)
								AND 		a0.actor_id <> %d) AS a, movie m, role r, genre g, movie_has_genre mhg
					WHERE	   a.actor_id = r.actor_id
					AND		   m.movie_id = r.movie_id
					AND		   g.genre_id = mhg.genre_id
					AND 	   m.movie_id = mhg.movie_id
					GROUP BY   a.actor_id;
					''' % (inp_id, inp_id, inp_id)
		# execute the query
		rows_affected = cur.execute(sql_query)
		if rows_affected == 0:
			# return empty table, input actor is a chameleon
			return table
		# else, general case :
		data = cur.fetchall()
		for row in data:
			if (row[1] + inp_dif_genres) >= 7:
				# get actor_id
				a = row[0]
				# convert it to a list with just one element
				b = [(a)]
				# append it at the results table
				table.append(b)
	# something went wrong throw exception
	except:
		return [("Status",),("Error : A problem occured while executing the query!",),]	
	# return the results table to the website caller
	return table


def selectTopNactors(n): 
	#========================================================================================#					 
	''' Contract Below :
	4.selectTopNactors: Η συνάρτηση αυτή δέχεται ως όρισμα έναν ακέραιο Ν. Για κάθε
	είδος ταινίας, βρίσκει τους N ηθοποιούς που έχουν παίξει στις περισσότερες ταινίες
	του είδους. Επιστρέφει τα εξής:
	○ To όνομα του είδους.
	○ Toν κωδικο κάθε ηθοποιού.
	○ Τον αριθμό των ταινιών του είδους στις οποίες έχει παίξει ο κάθε ηθοποιός.
	'''
	#========================================================================================#
	# create a new connection
	con=connection()
	# create a cursor on the connection
	cur=con.cursor()
	# initial input check, only integers accepted
	try:
		check = int(n)
		# secondary check, only positive integers accepted
		if check < 0:
			return [("Status",),("Error : Input integer must be non-negative!",),]
		# third check, -0 is unique input :P	
		if n == '-0':
			return [("Status",),("Error : Input integer must be non-negative!",),]
	except ValueError as v:
		print(v)
		return [("Status",),("Error : Input must be an integer!",),]
	# define the query to the database
	# treaty : a.actor_id = ASC so that results are not different each time on equal \
	# num of movies , we choose actors with lower ID first
	sql_query = '''
				SELECT   g.genre_name, a.actor_id, count(mhg.movie_id) AS dif_movies
				FROM	 genre g, actor a, movie m, role r, movie_has_genre mhg
				WHERE	 g.genre_id = mhg.genre_id
				AND		 mhg.movie_id = m.movie_id
				AND		 a.actor_id = r.actor_id
				AND		 r.movie_id = m.movie_id
				GROUP BY g.genre_name, a.actor_id
				ORDER BY g.genre_name, dif_movies DESC, a.actor_id ASC;
				'''
	# initialise the results table, first row = header			
	table = 	[("genreName", "actorId", "numberOfMovies"),]
	i = 0 		# index for x
	x = str(i)	# rows/genre converted to string, used as counter
	k = 1		# index for z
	z = str(k)	# same like x but used for case n = 1 only
	j = 0		# genre counter = 21 dif_genres in our DB
	l = 0 		# counts rows fetched
	# execute the sql query	
	try:
		# execute() returns number of rows found
		rows_affected = cur.execute(sql_query)
		# 1st case : n = 0
		if x == n:
		# return empty table	
			return table
		# 2nd case : n = 1	
		elif z == n:
			while j < 21:
				j += 1
				row = cur.fetchone()
				l += 1
				genre = row[0]
				y = row[0]
				while (y == genre) and (l < rows_affected):
					temp_row = cur.fetchone()
					l += 1
					y = temp_row[0]
				if j < 2:
					table.append(row)
				if j != 21:
					table.append(temp_row)			
		# 3rd case : n > 1	
		else:
			while j < 21:
				j += 1
				row = cur.fetchone()
				l += 1
				table.append(row)
				genre = row[0]
				y = row[0]
				flag = 0
				# reminder : inut n != integer, its a string so have to convert index i into string x for comparison
				i += 1
				x = str(i)
				# add rows until n rows added, genre changes, or out of rows
				while (x != n) and (flag == 0) and (l < rows_affected):
					row = cur.fetchone()
					l += 1
					table.append(row)
					if y != genre:
						flag = 1
					i += 1
					x = str(i)
				# skip rows until next genre	
				while (y == genre) and (l < rows_affected):
					row = cur.fetchone()
					l += 1
					y = row[0]
				# if j = 21 we are done with genres ,no need adding the last row (it will be +1 after n)
				if j != 21:
					table.append(row)
				# reinitialise indexes from 1 since first row of new genre is already added
				i = 1
				x = str(i)
		# return the results table to the website caller
		return table
	# something went wrong throw exception
	except:
		return [("Status",),("Error : A problem occured while executing the query!",),]
