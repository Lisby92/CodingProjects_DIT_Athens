# --------------------------------------
# --------------------------------------
DROP PROCEDURE IF EXISTS ValidateQuery;
DELIMITER //
CREATE PROCEDURE ValidateQuery(IN qNum INT, IN queryTableName VARCHAR(255))
BEGIN
	DECLARE cname VARCHAR(64);
	DECLARE done INT DEFAULT FALSE;
	DECLARE cur CURSOR FOR SELECT c.column_name FROM information_schema.columns c WHERE 
c.table_schema='movies' AND c.table_name=queryTableName;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	# Add the column fingerprints into a tmp table
	DROP TABLE IF EXISTS cFps;
	CREATE TABLE cFps (
  	  `val` VARCHAR(50) NOT NULL
	) 
	ENGINE = InnoDB;

	OPEN cur;
	read_loop: LOOP
		FETCH cur INTO cname;
		IF done THEN
      			LEAVE read_loop;
    		END IF;
		
		DROP TABLE IF EXISTS ordered_column;
		SET @order_by_c = CONCAT('CREATE TABLE ordered_column as SELECT ', cname, ' FROM ', queryTableName, ' ORDER BY ', cname);
		PREPARE order_by_c_stmt FROM @order_by_c;
		EXECUTE order_by_c_stmt;
		
		SET @query = CONCAT('SELECT md5(group_concat(', cname, ', "")) FROM ordered_column INTO @cfp');
		PREPARE stmt FROM @query;
		EXECUTE stmt;

		INSERT INTO cFps values(@cfp);
		DROP TABLE IF EXISTS ordered_column;
	END LOOP;
	CLOSE cur;

	# Order fingerprints
	DROP TABLE IF EXISTS oCFps;
	SET @order_by = 'CREATE TABLE oCFps as SELECT val FROM cFps ORDER BY val'; 
	PREPARE order_by_stmt FROM @order_by;
	EXECUTE order_by_stmt;

	# Read the values of the result
	SET @q_yours = 'SELECT md5(group_concat(val, "")) FROM oCFps INTO @yours';
	PREPARE q_yours_stmt FROM @q_yours;
	EXECUTE q_yours_stmt;

	SET @q_fp = CONCAT('SELECT fp FROM fingerprints WHERE qnum=', qNum,' INTO @rfp');
	PREPARE q_fp_stmt FROM @q_fp;
	EXECUTE q_fp_stmt;

	SET @q_diagnosis = CONCAT('select IF(@rfp = @yours, "OK", "ERROR") into @diagnosis');
	PREPARE q_diagnosis_stmt FROM @q_diagnosis;
	EXECUTE q_diagnosis_stmt;

	INSERT INTO results values(qNum, @rfp, @yours, @diagnosis);

	DROP TABLE IF EXISTS cFps;
	DROP TABLE IF EXISTS oCFps;
END//
DELIMITER ;

# --------------------------------------

# Execute queries (Insert here your queries).

# Validate the queries
drop table if exists results;
CREATE TABLE results (
  `qnum` INTEGER  NOT NULL,
  `rfp` VARCHAR(50)  NOT NULL,
  `yours` VARCHAR(50)  NOT NULL,
  `diagnosis` VARCHAR(10)  NOT NULL
)
ENGINE = InnoDB;


# -------------
# Q1
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select  m.title
from 	movie m, movie_has_genre mhg, genre g, role r, actor a
where 	mhg.movie_id = m.movie_id 
	and g.genre_id = mhg.genre_id
	and g.genre_name = 'Comedy'
    and m.movie_id = r.movie_id
    and r.actor_id = a.actor_id
	and	a.last_name = 'Allen';

CALL ValidateQuery(1, 'q');
drop table if exists q;
# -------------


# -------------
# Q2
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select   d.last_name , m.title
from 	 (select	 d1.*
		  from	 director d1, movie m1, movie_has_director mhd1, movie_has_genre mhg1, genre g1
		  where	 m1.movie_id = mhd1.movie_id
		  and		 mhd1.director_id = d1.director_id
		  and		 g1.genre_id = mhg1.genre_id
		  and		 mhg1.movie_id = m1.movie_id
		  group by d1.director_id
		  having   count(*) > 1) AS d, movie m, movie_has_director mhd, role r, actor a
where	 m.movie_id = mhd.movie_id
and	 	 d.director_id = mhd.director_id
and  	 m.movie_id = r.movie_id
and  	 r.actor_id = a.actor_id
and	 	 a.last_name = 'Allen'
group by d.last_name, m.title;


CALL ValidateQuery(2, 'q');
drop table if exists q;
# -------------


# -------------
# Q3
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select distinct a.last_name
from   (select a1a.*
		from   actor a1a, role r1a, movie_has_director mhd1a, director d1a, movie m1a
		where  m1a.movie_id = r1a.movie_id
		and	   a1a.actor_id = r1a.actor_id
		and	   m1a.movie_id = mhd1a.movie_id
		and	   mhd1a.director_id = d1a.director_id
		and	   d1a.last_name = a1a.last_name)as a,
	   (select distinct g1b.genre_id
		from   genre g1b, movie_has_genre mhg1b, actor a1b, role r1b, movie m1b, movie_has_director mhd1b, director d1b
        where  a1b.last_name = d1b.last_name
        and	   a1b.actor_id <> r1b.actor_id
        and	   r1b.movie_id = m1b.movie_id
        and	   m1b.movie_id = mhd1b.movie_id
        and	   d1b.director_id = mhd1b.director_id
        and	   g1b.genre_id = mhg1b.genre_id
        and	   mhg1b.movie_id = m1b.movie_id) as g ,
        role r, movie_has_director mhd, director d, movie m, movie_has_genre mhg
where  m.movie_id = r.movie_id
and	   a.actor_id = r.actor_id
and	   m.movie_id = mhd.movie_id
and	   mhd.director_id = d.director_id
and	   d.last_name <> a.last_name
and	   mhg.movie_id = m.movie_id
and	   mhg.genre_id = g.genre_id;

CALL ValidateQuery(3, 'q');
drop table if exists q;
# -------------


# -------------
# Q4
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select distinct 'yes' as answer
from movie
where exists (select *
from	movie m, genre g, movie_has_genre mhg
where	mhg.movie_id = m.movie_id
and		g.genre_id = mhg.genre_id
and		g.genre_name = 'Drama'
and		m.year = 1995)
union all
select distinct 'no' as answer
from movie
where not exists (select *
from	movie m, genre g, movie_has_genre mhg
where	mhg.movie_id = m.movie_id
and		g.genre_id = mhg.genre_id
and		g.genre_name = 'Drama'
and		m.year = 1995);

CALL ValidateQuery(4, 'q');
drop table if exists q;
# -------------


# -------------
# Q5
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select  d1.last_name as d1, d2.last_name as d2
from    (select   d1a.director_id, d1a.last_name
		 from     director d1a, movie m1a, movie_has_director mhd1a, genre g1a, movie_has_genre mhg1a
		 where    d1a.director_id = mhd1a.director_id
		 and	     mhd1a.movie_id = m1a.movie_id
		 and	     g1a.genre_id = mhg1a.genre_id
		 and	     mhg1a.movie_id = m1a.movie_id
		 group by d1a.director_id, d1a.last_name
		 having count(distinct mhg1a.genre_id) >= 6) as d1,
		(select   d1b.director_id, d1b.last_name
		 from     director d1b, movie m1b, movie_has_director mhd1b, genre g1b, movie_has_genre mhg1b
		 where    d1b.director_id = mhd1b.director_id
		 and	     mhd1b.movie_id = m1b.movie_id
		 and	     g1b.genre_id = mhg1b.genre_id
		 and	     mhg1b.movie_id = m1b.movie_id
		 group by d1b.director_id, d1b.last_name
		 having count(distinct mhg1b.genre_id) >= 6) as d2,
		movie_has_director mhd1, movie_has_director mhd2, movie m
where   d1.director_id < d2.director_id
and	   mhd1.director_id = d1.director_id
and	   mhd2.director_id = d2.director_id
and	   m.movie_id = mhd1.movie_id
and	   m.movie_id = mhd2.movie_id
and	   m.year	between 2000 and 2006;

CALL ValidateQuery(5, 'q');
drop table if exists q;
# -------------


# -------------
# Q6
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select a.first_name, a.last_name, count(distinct mhd.director_id) as num_of_directors
from   (select  a1.first_name, a1.last_name, a1.actor_id, a1.gender, count(distinct m1.movie_id) as dif_movies
		from 	actor a1, movie m1, role r1
		where	a1.actor_id = r1.actor_id
		and		m1.movie_id = r1.movie_id
		group by a1.first_name, a1.last_name, a1.actor_id, a1.gender
		having dif_movies = 3) as a, movie_has_director mhd, director d, movie m, role r
where	a.actor_id = r.actor_id
and		m.movie_id = mhd.movie_id
and		d.director_id = mhd.director_id
and		r.movie_id = m.movie_id
group by a.first_name, a.last_name;

CALL ValidateQuery(6, 'q');
drop table if exists q;
# -------------


# -------------
# Q7
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select	 g.genre_id, count(distinct mhd.director_id) as dif_directors
from	 (select	 distinct g1.*
		  from	 (select 	 m2.movie_id
				  from 	 	 movie m2, movie_has_genre mhg2, genre g2
				  where 	 m2.movie_id = mhg2.movie_id
				  and		 g2.genre_id = mhg2.genre_id
				  group by 	 m2.movie_id
				  having 	 count(distinct mhg2.genre_id) = 1) as m1,
          genre g1, movie_has_genre mhg1
		  where	  g1.genre_id = mhg1.genre_id
		  and		  m1.movie_id = mhg1.movie_id) as g,
		 movie m, movie_has_director mhd, director d, movie_has_genre mhg
where	 g.genre_id = mhg.genre_id
and		 m.movie_id = mhg.movie_id
and		 m.movie_id = mhd.movie_id
and		 d.director_id = mhd.director_id
group by g.genre_id;

CALL ValidateQuery(7, 'q');
drop table if exists q;
# -------------


# -------------
# Q8
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select a1.actor_id
from(select a.actor_id, count(distinct mhg.genre_id) as dif_genres
	from   actor a, movie_has_genre mhg, role r, genre g, movie m
	where  a.actor_id = r.actor_id
	and	   m.movie_id = r.movie_id
	and	   g.genre_id = mhg.genre_id
	and	   m.movie_id = mhg.movie_id
	group by a.actor_id) as a1
where a1.dif_genres = (select count(distinct g.genre_id) from genre g);

CALL ValidateQuery(8, 'q');
drop table if exists q;
# -------------


# -------------
# Q9
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select g1, g2, count(dif_directors1) as dif_directors 
from (select g1.genre_id as g1, g2.genre_id as g2, mhd1.director_id as dif_directors1
	  from   genre g1, genre g2, movie m1, movie m2, movie_has_director mhd1, movie_has_director mhd2, movie_has_genre mhg1, movie_has_genre mhg2, director d
	  where  g1.genre_id < g2.genre_id
	  and	   g1.genre_id = mhg1.genre_id
	  and	   g2.genre_id = mhg2.genre_id
	  and	   mhg1.genre_id <> mhg2.genre_id
	  and	   mhg1.movie_id = m1.movie_id
	  and	   mhg2.movie_id = m2.movie_id
	  and	   m1.movie_id <> m2.movie_id
	  and	   d.director_id = mhd1.director_id
	  and	   d.director_id = mhd2.director_id
	  and	   mhd1.movie_id = m1.movie_id
	  and	   mhd2.movie_id = m2.movie_id
	  group by g1.genre_id, g2.genre_id, mhd1.director_id
      union
	  select g1.genre_id as g1, g2.genre_id as g2,  mhd.director_id as dif_directors2
	  from   genre g1, genre g2, movie m, movie_has_director mhd, movie_has_genre mhg1, movie_has_genre mhg2, director d
	  where  g1.genre_id < g2.genre_id
	  and	   g1.genre_id = mhg1.genre_id
	  and	   g2.genre_id = mhg2.genre_id
	  and	   mhg1.movie_id = m.movie_id
	  and	   mhg2.movie_id = m.movie_id
	  and	   mhg1.movie_id = mhg2.movie_id
	  and	   d.director_id = mhd.director_id
	  and	   mhd.movie_id = m.movie_id
	  group by g1.genre_id, g2.genre_id,mhd.director_id ) as res
group by g1, g2;

CALL ValidateQuery(9, 'q');
drop table if exists q;
# -------------


# -------------
# Q10
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select * from actor a where a.actor_id = 933;

CALL ValidateQuery(10, 'q');
drop table if exists q;
# -------------

DROP PROCEDURE IF EXISTS RealValue;
DROP PROCEDURE IF EXISTS ValidateQuery;
DROP PROCEDURE IF EXISTS RunRealQueries;

select *
from results;
#queries by : VASILEIOS MAYROMMATHS 1115201200106