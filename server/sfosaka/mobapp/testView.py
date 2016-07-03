import datetime
import dateutil
from django.utils import timezone
from django.utils import dateparse
import time

import json

from django.test import TestCase

oneMonth = datetime.timedelta(days=30)
monthFromNow = timezone.now() + oneMonth

class StartViewTests(TestCase):
    fixtures = ['mobdata.json']

    def testGetMetaData(self):
        url = '/mobapp/start/'
        response = self.client.post(url, json.dumps({}),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'],True)

class PartnerViewTests(TestCase):
    fixtures = ['mobdata.json', 'partners.json']

    FirstPartnerDate  =  '2016-06-01T01:23:47Z'
    MiddleDate        =  '2016-06-01T01:24:01Z'
    #SecondPartnerDate = '2016-06-01T01:24:47Z'
    SecondPartnerDate = '2016-06-01T01:26:08+00:00'
    LateDate          = '2016-06-01T01:30:08+00:00'

    def _GetMeta(self, asOfDate=None, expectUpdate=True):
        url = '/mobapp/partner_meta/'

        content = {}
        if asOfDate is not None:
            content['as_of_date'] = asOfDate.isoformat()

        response = self.client.post(url, json.dumps(content),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'],True)
        self.assertEquals(contents['update_needed'],expectUpdate)

        return dateparse.parse_datetime(contents['update_from_date'])

    def testGetMetaData(self):
        # request without asOfDate
        serverDate = self._GetMeta()
        refDate = dateparse.parse_datetime(self.FirstPartnerDate)
        self.assertEquals(serverDate, refDate)

        # request with asOfDate after the first one, before the second
        asOf = dateparse.parse_datetime(self.MiddleDate)
        serverDate = self._GetMeta(asOf)
        refDate = dateparse.parse_datetime(self.SecondPartnerDate)
        self.assertEquals(serverDate, refDate)

        # request late date, so no update needed
        asOf = dateparse.parse_datetime(self.LateDate)
        serverDate = self._GetMeta(asOf, False)
        refDate = dateparse.parse_datetime(self.SecondPartnerDate)
        self.assertEquals(serverDate, refDate)

    def testGetData(self):
        url = '/mobapp/partner_data/'
        # request without asOfDate
        response = self.client.post(url, json.dumps({}),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'],True)
        partners = contents['partners_list']

        self.assertEquals(len(partners), 2)
        self.assertEquals(partners['1']['name'],'Kinokuniya')
        self.assertEquals(partners['2']['name'],'Yamasho')

class TranslatorViewTests(TestCase):
    fixtures = ['mobdata.json', 'translator.json']

    FirstWordDate  = '2016-06-07T23:13:52+00:00'
    MiddleDate     = '2016-06-07T23:15:45+00:00'
    LastDate       = '2016-06-07T23:24:51+00:00'

    def _GetMeta(self, asOfDate=None, expectUpdate=True):
        url = '/mobapp/dict_meta/'

        content = {}
        if asOfDate is not None:
            content['as_of_date'] = asOfDate.isoformat()

        response = self.client.post(url, json.dumps(content),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'],True)
        self.assertEquals(contents['update_needed'],expectUpdate)

        return dateparse.parse_datetime(contents['update_from_date'])

    def testGetMetaData(self):
        # request without asOfDate
        serverDate = self._GetMeta()
        refDate = dateparse.parse_datetime(self.FirstWordDate)
        self.assertEquals(serverDate, refDate)

        # request with asOfDate after the first one, before the second
#        asOf = dateparse.parse_datetime(self.MiddleDate)
#        serverDate = self._GetMeta(asOf)
#        refDate = dateparse.parse_datetime(self.MiddleDate)
#        self.assertEquals(serverDate, refDate)

        # request late date, so no update needed
        asOf = dateparse.parse_datetime(self.LastDate)
        serverDate = self._GetMeta(asOf, False)
        refDate = dateparse.parse_datetime(self.LastDate)
        self.assertEquals(serverDate, refDate)

    def testGetData(self):
        url = '/mobapp/dict_data/'
        # request without asOfDate
        response = self.client.post(url, json.dumps({}),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'],True)
        words = contents['words_list']
        enWords = {}
        jpWords = {}
        for w in words.values():
            langCode = w['language']
            if langCode == 'jp':
                jpWords[w['word']] = w['translations']
            elif langCode == 'en':
                enWords[w['word']] = w['translations']
            else:
                print('unknown lang code: %s' % langCode)

        self.assertEquals(len(jpWords),3)
        self.assertEquals(len(enWords),4)

    def testSubmitData(self):
        url = '/mobapp/dict_submit/'
        word = '勉強して'
        trans = ['discount, please']

        contents = {}
        contents['submit_word'] = word
        contents['submit_trans'] = trans

        response = self.client.post(url, json.dumps(contents),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'], True)
