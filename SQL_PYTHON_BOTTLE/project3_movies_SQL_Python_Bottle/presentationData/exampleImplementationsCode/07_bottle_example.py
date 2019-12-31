#coding: utf-8
from bottle import route, error, post, get, run, static_file, abort, redirect, response, request, template

@route('/')
@route('/index.html')
def index():
    return '<a href="/hello">Go to Hello World page</a>'

@route('/hello')
def hello():
    return '<h1>HELLO WOLRD</h1>'

@route('/hello/:name')
def hello_name(name):
    page = request.GET.get('page', '1')
    return '<h1>HELLO %s <br/>(%s)</h1>' % (name, page)


@route('/redirect')
def redirect_to_hello():
    redirect('/hello')


@get('/upload')
def upload_view():
    return """
        <form action="/upload" method="post" enctype="multipart/form-data">
          <input type="text" name="name" />
          <input type="file" name="data" />
          <input type="submit" name="submit" value="upload now" />
        </form>
        """    

@post('/upload')
def do_upload():
    name = request.forms.get('name')
    data = request.files.get('data')
    if name is not None and data is not None:
        raw = data.file.read() # small files =.=
        filename = data.filename
        return "Hello %s! You uploaded %s (%d bytes)." % (name, filename, len(raw))
    return "You missed a field."


run(host='localhost', port=8080, reloader=True)