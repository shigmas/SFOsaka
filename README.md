# SFOsaka
The client and server for the SF Osaka Sister city app. The letters SFO should be pronounced, not said, so it all comes out as one word.

## 2 Components:
 - Django/Python backend
 - Qt 5.x Android/iOS app

## Building and Installing:
### Server:
 1. Clone the repo
 2. Get a copy of machine_conf.py from someone
 3. <BASE>/server% virtualenv . -p python3
 4. <BASE>/server% bin/pip install -r requirements.txt

You will have to create your machine_conf.py for settings that aren't in settings.py:
 - DEBUG
 - SECRET_KEY
 - STATIC_ROOT (this is only necessary to generate the static files)
 - DATABASES

#### Database
Setup the 'database'. (The database will depend on your machine_conf.py, but the instructions should be the same)
 1. <BASE>/server/sfosaka% ../bin/python manage.py makemigrations (should be noop if you're installing for the first time)
 2. <BASE>/server/sfosaka% ../bin/python manage.py migrate
 3. <BASE>/server/sfosaka% ../bin/python manage.py syncdb

(On updates, skip makemigrations, since those should be checked in)

#### Static files
Setup the static files (in this case, only the admin .css files)
 1. <BASE>/server/sfosaka% ../bin/python manage.py collectstatic
 This will place the files in STATIC_ROOT (according to your machine_conf.py)

#### Backend server (behind apache or nginx)
It should be ready to serve. requirements.txt installs gunicorn, but for localhost serving, django has its own server
 - <BASE>/server/sfosaka% ../bin/python manage.py runserver
For gunicorn:
 - <BASE>/server/sfosaka% ../bin/gunicorn -w 4 -b :8001 sfosaka.wsgi:application
(This should be run through apache or nginx)

#### Front end
We're using nginx. The configuration isn't covered here, but in addition to the standard django configuration, you need to point to the static files. (I'm sure there's a better way)

    location /static {
        root <STATIC_ROOT>; #this is the path in from machine_conf.py, minus the 'static' part.
    }


### Client:
Prerequites: Qt 5.x
 1. Build FJClient. This is the Futomen JSON client. Just some basic HTTP/JSON marshalling
 2. Build SFOSaka
This should be repeated for every client. SFOContext.cpp has the servername in it.
 3. mapbox requires an API key which is not checked in. Until I figure out how to put it in a separate file, you have to edit the MapPage.qml file

Android notes:
OpenSSL needs to be built. See the Qt specific [note](http://doc.qt.io/qt-5/opensslsupport.html), but I found that the OpenSSL [setup](https://wiki.openssl.org/index.php/Android) more useful to configure.