import datetime
import dateutil
from django.utils import timezone
from django.utils import dateparse
import time

import json

from django.test import TestCase

from mobapp import models

oneMonth = datetime.timedelta(days=30)
monthFromNow = timezone.now() + oneMonth

class StartViewTests(TestCase):

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
    LastDate          = '2016-07-14T16:22:57Z'

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
        refDate = dateparse.parse_datetime(self.LastDate)
        self.assertEquals(serverDate, refDate)

        # request late date, so no update needed
        asOf = dateparse.parse_datetime(self.LastDate)
        serverDate = self._GetMeta(asOf, False)
        refDate = dateparse.parse_datetime(self.LastDate)
        self.assertEquals(serverDate, refDate)

    def testGetData(self):
        url = '/mobapp/partner_data/'
        # request without asOfDate
        response = self.client.post(url, json.dumps({}),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'],True)
        partners = contents['partners_list']

        self.assertEquals(len(partners), 3)
        self.assertEquals(partners['1']['name'],'Kinokuniya')
        self.assertEquals(partners['2']['name'],'Yamasho')
        self.assertEquals(partners['3']['name'],'Consulate General of Japan')

        # Check the contact info for yamasho
        contact = partners['2']['contact']
        self.assertEquals(contact['street_number'],'1161 Post St',
                          'Yamasho street address does not match')

class PerformerViewTests(TestCase):
    # The logic for this is identical to Partners, so we won't duplicate
    # everything. Just making sure it can handle the key that's really different
    # (status)
    fixtures = ['mobdata.json', 'performers.json']
    PerformerDate = '2016-07-23T21:23:05.370000+00:00'
    PerformerUrl = 'https://s3-us-west-1.amazonaws.com/sfosaka/images/IMG_4892.jpg'
    StartTime = '2016-08-27T14:00:00Z'
    EndTime = '2016-08-27T14:20:00Z'
    def _GetMeta(self, asOfDate=None, expectUpdate=True):
        url = '/mobapp/performer_meta/'

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
        refDate = dateparse.parse_datetime(self.PerformerDate)
        self.assertEquals(serverDate, refDate)

    def testGetData(self):
        url = '/mobapp/performer_data/'
        # request without asOfDate
        response = self.client.post(url, json.dumps({}),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'],True)
        performers = contents['performers_list']

        self.assertEquals(len(performers), 1)
        self.assertEquals(performers['1']['name'],'Ken Takeda')

        # Check the contact info for yamasho
        contact = performers['1']['status']
        self.assertEquals(contact,'active', 'Unexpected status')

        imageUrl = performers['1']['imageUrl']
        self.assertEquals(imageUrl,self.PerformerUrl, 'Unexpected image Url')

        startTime = dateparse.parse_datetime(self.StartTime)
        endTime = dateparse.parse_datetime(self.EndTime)

        self.assertEquals(dateparse.parse_datetime(performers['1']['startTime']),
                          startTime,
                          'Unexpected start time');
        self.assertEquals(dateparse.parse_datetime(performers['1']['endTime']),
                          endTime,
                          'Unexpected end time');

class TranslatorViewTests(TestCase):
    fixtures = ['mobdata.json', 'translator.json']

    FirstWordDate  = '2016-06-07T23:13:52+00:00'
    MiddleDate     = '2016-06-07T23:15:45+00:00'
    LastDate       = '2016-06-07T23:24:51+00:00'

    word = '勉強して'
    wordPhonetic = 'benkyoushite'
    transDict = {'discount, please':'discount, please'}

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

    def _verifySubmitWord(self, word, phonetic = None):
        try:
            entry = models.DictionaryWord.objects.get(word=word)
        except:
            pass

        pendingStatus = models.Status.objects.get(name='pending')
        
        self.assertEquals(entry.word, word, 'Word not found')
        self.assertEquals(entry.status, pendingStatus, 'Status is not pending')
        if phonetic:
            self.assertEquals(entry.phonetic, phonetic,
                              'Phonetic word does not match')
        

    def testSubmitDataList(self):
        url = '/mobapp/dict_submit/'

        contents = {}
        contents['submit_word'] = self.word
        contents['submit_trans'] = list(self.transDict.keys())

        response = self.client.post(url, json.dumps(contents),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'], True)

        self._verifySubmitWord(self.word)
        for tran in self.transDict:
            self._verifySubmitWord(tran)

    def testSubmitDataDict(self):
        url = '/mobapp/dict_submit/'

        contents = {}
        contents['submit_word'] = self.word
        contents['submit_word_phonetic'] = self.wordPhonetic
        contents['submit_trans'] = self.transDict

        response = self.client.post(url, json.dumps(contents),
                                    content_type='application/json')
        contents = json.loads(str(response.content, encoding='utf-8'))
        self.assertEquals(contents['result'], True)

        self._verifySubmitWord(self.word, self.wordPhonetic)
        for tran in self.transDict:
            self._verifySubmitWord(tran, self.transDict[tran])
