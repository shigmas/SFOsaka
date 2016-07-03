
# Do not check this in!

import os
from os.path import expanduser

# SECURITY WARNING: keep the secret key used in production secret!
SECRET_KEY = 'u36-f@p7t=w&g*pjr4u9nu%11^v_2l+4i#fn6baje96f-f6rs0'

BASE_DIR = os.path.dirname(os.path.dirname(__file__))


DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.sqlite3',
        'NAME': os.path.join(BASE_DIR, 'db.sqlite3'),
    }
}
