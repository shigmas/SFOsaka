# SFOsaka
The client and server for the SF Osaka Sister city app.

Django/Python backend
Qt 5.x Android/iOS app

Building and Installing:
Server:
1. Clone the repo
2. Get a copy of machine_conf.py from someone
3. <BASE>/server% virtualenv . -p python3
4. <BASE>/server% bin/pip install -r requirements.txt


Setup the 'database'. (The database will depend on your machine_conf.py, but the instructions should be the same)
1. <BASE>/server/sfosaka% ../bin/python manage.py makemigrations (should be noop if you're installing for the first time)
2. <BASE>/server/sfosaka% ../bin/python manage.py migrate
3. <BASE>/server/sfosaka% ../bin/python manage.py syncdb

It should be ready to serve. requirements.txt installs gunicorn, but for localhost serving, django has its own server
- <BASE>/server/sfosaka% ../bin/python manage.py runserver
For gunicorn:
- <BASE>/server/sfosaka% ../bin/gunicorn -w 4 -b :8001 sfosaka.wsgi:application
(This should be run through apache or nginx)
