from django.conf.urls import patterns, url

# It's a pretty straight forward mapping, and we don't want to have import
# every class separately.
#from skate.View import QueryView, UserView
import mobapp.View

urlpatterns = patterns('',
                       url(r'^start/$',
                           mobapp.View.StartView.as_view(command='start')),
                       url(r'^partner_meta/$',
                           mobapp.View.PartnerView.as_view(command='fetch_partner_meta')),
                       url(r'^partner_data/$',
                           mobapp.View.PartnerView.as_view(command='fetch_partner_data')),
                       url(r'^performer_meta/$',
                           mobapp.View.PerformerView.as_view(command='fetch_performer_meta')),
                       url(r'^performer_data/$',
                           mobapp.View.PerformerView.as_view(command='fetch_performer_data')),

                       url(r'^dict_meta/$',
                           mobapp.View.TranslatorView.as_view(command='fetch_words_meta')),
                       url(r'^dict_data/$',
                           mobapp.View.TranslatorView.as_view(command='fetch_words_data')),
                       url(r'^dict_submit/$',
                           mobapp.View.TranslatorView.as_view(command='submit_words_data')),
                       
)
