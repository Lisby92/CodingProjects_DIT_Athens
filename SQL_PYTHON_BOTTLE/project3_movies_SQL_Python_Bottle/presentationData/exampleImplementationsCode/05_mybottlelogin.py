from bottle import run, post, route, request

@route('/login')
def login():
    return '''
        <form action="/login" method="post">
            Username: <input name="username" type="text" />
            Password: <input name="password" type="password" />
            <input value="Login" type="submit" />
        </form>
    '''

@route('/login', method='POST')
def do_login():
    username = request.forms.get('username')
    password = request.forms.get('password')
    return "Name : "+request.method+" username: "+username+" password:   "+password
    
 

run(host='localhost', port=8080, debug=True)

 