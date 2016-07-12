from django.conf.urls import include, url
from django.contrib import admin

from django.contrib.staticfiles.urls import staticfiles_urlpatterns

urlpatterns = [
    # Examples:
    # url(r'^$', 'sfosaka.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    url(r'^admin/', include(admin.site.urls)),
    url(r'^mobapp/', include('mobapp.urls')),
]

urlpatterns += staticfiles_urlpatterns()
