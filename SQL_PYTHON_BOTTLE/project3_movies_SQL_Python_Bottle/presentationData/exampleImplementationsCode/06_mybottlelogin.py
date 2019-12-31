from bottle import run, get, route, request

#The query string (as in /forum?id=1&page=5) is commonly used to transmit a small number of key/value pairs to the server.
@route('/login', method='GET')
def login():
    return '''
        <form action="/page" method="get">
            Firstname: <input name="firstname" type="text" />
            Lastname: <input name="lastname" type="text" />
            <input type="submit" value="GO" />
        </form>
    '''

@route('/page', method='GET')
def do_login():
    name = request.query.firstname
    last = request.query.lastname
    return "Name : "+request.query.method+" firstname: "+name+" lastname:   "+last
    

run(host='localhost', port=8080, debug=True)

 