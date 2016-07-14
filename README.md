# SFOsaka
The client and server for the SF Osaka Sister city app. The letters SFO should be pronounced, not said, so it all comes out as one word.

Django/Python backend
Qt 5.x Android/iOS app

Building and Installing:
Server:
 1. Clone the repo
 2. Get a copy of machine_conf.py from someone
 3. <BASE>/server% virtualenv . -p python3
 4. <BASE>/server% bin/pip install -r requirements.txt

You will have to create your machine_conf.py for settings that aren't in settings.py:
 - SECRET_KEY
 - DATABASES

Setup the 'database'. (The database will depend on your machine_conf.py, but the instructions should be the same)
 1. <BASE>/server/sfosaka% ../bin/python manage.py makemigrations (should be noop if you're installing for the first time)
 2. <BASE>/server/sfosaka% ../bin/python manage.py migrate
 3. <BASE>/server/sfosaka% ../bin/python manage.py syncdb

(On updates, skip makemigrations, since those should be checked in)

It should be ready to serve. requirements.txt installs gunicorn, but for localhost serving, django has its own server
 - <BASE>/server/sfosaka% ../bin/python manage.py runserver
For gunicorn:
 - <BASE>/server/sfosaka% ../bin/gunicorn -w 4 -b :8001 sfosaka.wsgi:application
(This should be run through apache or nginx)


Client:
Prerequites: Qt 5.x
 1. Build FJClient. This is the Futomen JSON client. Just some basic HTTP/JSON marshalling
 2. Build SFOSaka
This should be repeated for every client.
 3. mapbox requires an API key which is not checked in. Until I figure out how to put it in a separate file, you have to edit the MapPage.qml file

Android notes:
OpenSSL needs to be built. See the Qt specific [note](http://doc.qt.io/qt-5/opensslsupport.html), but I found that the OpenSSL [setup](https://wiki.openssl.org/index.php/Android) more useful to configure.