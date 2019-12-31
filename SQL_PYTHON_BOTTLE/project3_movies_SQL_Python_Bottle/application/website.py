import sys, os
sys.path.append(os.path.join(os.path.split(os.path.abspath(__file__))[0], 'lib'))
from bottle import route, run, static_file, request
import pymysql as db
import settings
import app

def renderTable(tuples):
    # this function accepts a list that contains tuples
    # it then iterates through the list of tuples and creates an html table that displays the results
    # the first tuple in the list is going to be the table header, <th> element, and the rest of the values are written row-by-row
    # more about html tables here: https://www.w3schools.com/html/html_tables.asp
    # for the python join() function read here: https://www.tutorialspoint.com/python/string_join.htm	

    printResult = """<style type='text/css'> h1 {color:red;} h2 {color:blue;} p {color:green;} </style>
    <table border = '1' frame = 'above'>"""

    header='<tr><th>'+'</th><th>'.join([str(x) for x in tuples[0]])+'</th></tr>'
    data='<tr>'+'</tr><tr>'.join(['<td>'+'</td><td>'.join([str(y) for y in row])+'</td>' for row in tuples[1:]])+'</tr>'
        
    printResult += header+data+"</table>"
    return printResult

@route('/updateRank')
def updateRankWEB():
    r1 = request.query.rank1 or "Unknown Rank"
    r2 = request.query.rank2 or "Unknown Rank"
    mtitle = request.query.movieTitle or "Unknown Title"
    table = app.updateRank(r1, r2, mtitle)
    return "<html><body>" + renderTable(table) + "</body></html>"
	
@route('/colleaguesOfColleagues')
def colleaguesOfColleaguesWEB():
    aid1 = request.query.actorId1
    aid2 = request.query.actorId2
    table = app.colleaguesOfColleagues(aid1, aid2)
    return "<html><body>" + renderTable(table) + "</body></html>"

@route('/actorPairs')
def actorPairsWEB():
    aid = request.query.actorId
    table = app.actorPairs(aid)
    return "<html><body>" + renderTable(table) + "</body></html>"
	
@route('/selectTopNactors')
def selectTopNactorsWEB():
    n = request.query.n
    table = app.selectTopNactors(n)
    return "<html><body>" + renderTable(table) + "</body></html>"

  
@route('/:path')
def callback(path):
    return static_file(path, 'web')

@route('/')
def callback():
    return static_file("index.html", 'web')

run(host='localhost', port=settings.web_port, reloader=True, debug=True)
