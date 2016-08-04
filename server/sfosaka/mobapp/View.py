from django.http import JsonResponse
from django.http import HttpResponse
from django.http import HttpResponseRedirect
from django.views.generic import View

# Template libs
from django.shortcuts import render

import datetime
from django.utils import timezone
from django.utils import dateparse

import types

from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.csrf import ensure_csrf_cookie
from django.utils.decorators import method_decorator

import django.db.models.fields

from urllib.parse import urlparse


import os
import os.path
import json

# To convert from/to python datetime and the isoformat
import dateutil.parser
from django.utils import timezone

from mobapp.models import *
# https://gist.github.com/anonymous/2204527
import mobapp.unicodedata2

class BaseView(View):
    command = None
    version = None

    kMessageKey = 'message'
    kResultKey = 'result'

    kVersion1Key = 'v1'

    kAsOfDateKey = 'as_of_date'

    kUpdateFromDateKey = 'update_from_date'
    kNeedsUpdateKey = 'update_needed'

    def get(self, request):
        return self._handleCommand()

    def post(self, request):
        return self._handleCommand()

    def _getContent(self, request):
        content = None
        error = None
        # returns a dictionary from a POST request body in JSON format
        try:
            content = json.loads(str(request.body, encoding='utf-8'))
        except ValueError:
            error = 'Invalid JSON content in request body: (%s)' % content
            content = {}

        return content, error

class StartView(BaseView):    
    @method_decorator(ensure_csrf_cookie)
    def dispatch(self, *args, **kwargs):
        return super(StartView, self).dispatch(*args, **kwargs)

    def _handleCommand(self):
        return JsonResponse({self.kResultKey:True}, safe=True)

class OrganizationView(BaseView):

    # Must be overridden
    def _GetOrganizationSubclass(self):
        return None
    def _GetMetaDataCommand(self):
        return None
    def _GetDataCommand(self):
        return None
    def _GetUpdateFromDateKey(self):
        return None
    def _GetNeedsUpdateKey(self):
        return None
    def _GetListKey(self):
        return None

    def _handleCommand(self):
        content, error = self._getContent(self.request)

        commandDict = { self._GetMetaDataCommand() : self._getMetaData,
                        self._GetDataCommand() : self._getData,
                    }

        action = commandDict.get(self.command, None)
        if action is None:
            print('no command for %s' % self.command)
            return JsonResponse({}, safe=True)

        result, success = action(content)
        # return JSON result
        content = {}
        if success:
            if type(result) == type({}):
                content = result
        else:
            content[self.kMessageKey] = result
        content[self.kResultKey] = success
        return JsonResponse(content, safe=True)

    def _getMetaData(self, content):
        # It could be None.
        subclass = self._GetOrganizationSubclass()
        asOfDate = content.get(self.kAsOfDateKey, None)
        needsUpdate = True
        if asOfDate is None:
            organization = subclass.objects.earliest()
        else:
            organization = subclass.objects.latest()
            dt = dateparse.parse_datetime(asOfDate)
            if dt >= organization.modificationDate:
                needsUpdate = False
        results = {}
        results[self._GetUpdateFromDateKey()] = organization.modificationDate.isoformat()
        results[self._GetNeedsUpdateKey()] = needsUpdate

        return results, True

    def _getContactInfoDict(self, contact):
        contactDict = {}
        fields = contact._meta.concrete_fields
        for field in fields:
            attr = getattr(contact, field.name)
            if attr is not None:
                contactDict[field.name] = attr

        return contactDict
            
    def _getData(self, content):

        organizations = self._GetOrganizationSubclass().objects.all()
        orgsDict = {}
        for o in organizations:
            oDict = {}
            fields = o._meta.concrete_fields
            itemId = None
            for field in fields:
                # Shouldn't hardcode these fields, but maybe we don't
                # need them anywhere else? 
                if field.name == 'id':
                    itemId = int(getattr(o, field.name))
                # Test if a relationship. If so, add the name, since that's
                # the category
                if field.rel is None:
                    if type(field) is django.db.models.fields.DateTimeField:
                        dt = getattr(o, field.name)
                        oDict[field.name] = dt.isoformat()
                    else:
                        oDict[field.name] = getattr(o, field.name)
                elif field.name == 'contact':
                    contact = getattr(o, field.name)
                    if contact is not None:
                        oDict[field.name] = self._getContactInfoDict(contact)
                else:
                    attr = getattr(o, field.name)
                    if attr is not None:
                        oDict[field.name] = attr.name
                    
            if itemId is not None:
                orgsDict[itemId] = oDict

        content[self._GetListKey()] = orgsDict

        return content, True

class PartnerView(OrganizationView):
        # commands
    kGetMetaCommand = 'fetch_partner_meta'
    kGetDataCommand = 'fetch_partner_data'

    kPartnersListKey = 'partners_list'

    def _GetOrganizationSubclass(self):
        return Partner

    def _GetMetaDataCommand(self):
        return self.kGetMetaCommand

    def _GetDataCommand(self):
        return self.kGetDataCommand

    def _GetUpdateFromDateKey(self):
        return self.kUpdateFromDateKey

    def _GetNeedsUpdateKey(self):
        return self.kNeedsUpdateKey

    def _GetListKey(self):
        return self.kPartnersListKey

class PerformerView(OrganizationView):
        # commands
    kGetMetaCommand = 'fetch_performer_meta'
    kGetDataCommand = 'fetch_performer_data'

    kPerfomersListKey = 'performers_list'

    def _GetOrganizationSubclass(self):
        return Performer

    def _GetMetaDataCommand(self):
        return self.kGetMetaCommand

    def _GetDataCommand(self):
        return self.kGetDataCommand

    def _GetUpdateFromDateKey(self):
        return self.kUpdateFromDateKey

    def _GetNeedsUpdateKey(self):
        return self.kNeedsUpdateKey

    def _GetListKey(self):
        return self.kPerfomersListKey

class TranslatorView(BaseView):
        # commands
    kGetMetaCommand = 'fetch_words_meta'
    kGetDataCommand = 'fetch_words_data'
    kSubmitDataCommand = 'submit_words_data'

    kSubmitWordKey = 'submit_word'
    kSubmitWordPhoneticKey = 'submit_word_phonetic'
    kSubmitTranslationsKey = 'submit_trans'

    kTranslationsListKey = 'words_list'

    def _handleCommand(self):
        content, error = self._getContent(self.request)

        commandDict = { self.kGetMetaCommand : self._getMetaData,
                        self.kGetDataCommand : self._getData,
                        self.kSubmitDataCommand : self._submitData,
                    }
        action = commandDict.get(self.command, None)
        if action is None:
            print('no command for %s' % self.command)
            return JsonResponse({}, safe=True)

        result, success = action(content)
        # return JSON result
        content = {}
        if success:
            if type(result) == type({}):
                content = result
        else:
            content[self.kMessageKey] = result
        content[self.kResultKey] = success
        return JsonResponse(content, safe=True)

    def _getMetaData(self, content):
        # It could be None.
        asOfDate = content.get(self.kAsOfDateKey, None)
        needsUpdate = True
        if asOfDate is None:
            word = DictionaryWord.objects.earliest()
        else:
            word = DictionaryWord.objects.latest()
            dt = dateparse.parse_datetime(asOfDate)
            if dt >= word.modificationDate:
                needsUpdate = False
        results = {}
        results[self.kUpdateFromDateKey] = word.modificationDate.isoformat()
        results[self.kNeedsUpdateKey] = needsUpdate

        return results, True

    def _getData(self, content):
        content = {}
        allTrans = {}
        words = DictionaryWord.objects.all()
        latest = DictionaryWord.objects.latest().modificationDate.isoformat()
        content[self.kAsOfDateKey] = latest;
        activeStatus = Status.objects.get(name='active')
        for w in words:
            # Skip word if the status is not active
            if w.status != activeStatus:
                continue
            wDict = {}
            fields = w._meta.concrete_fields
            itemId = None
            for field in fields:
                # Shouldn't hardcode these fields, but maybe we don't
                # need them anywhere else? 
                if field.name == 'id':
                    itemId = int(getattr(w, field.name))
                # Test if a relationship. If so, add the name, since that's
                # the category
                if field.rel is None:
                    if type(field) is django.db.models.fields.DateTimeField:
                        dt = getattr(w, field.name)
                        wDict[field.name] = dt.isoformat()
                    else:
                        wDict[field.name] = getattr(w, field.name)
                elif field.name == "language":
                    wDict[field.name] = getattr(w, field.name).language_code
#                else: # we knowingly don't handle the status field
#                    print('unhandled field: %s' % field.name)
            # Translations is not a concrete field. It's many_to_many, so we
            # access it directly.
            translations = []
            for tWord in w.translations.all():
                translations.append(tWord.word)
            wDict['translations'] = translations

            if itemId is not None:
                allTrans[itemId] = wDict
        content[self.kTranslationsListKey] = allTrans
            
        return content, True

    # Returns a tuple: (word, bool), where bool is true if the word is a
    # japanese word
    def _CleanAndGetLang(self, word):
        # just strip any whitespace off from the beginning or end
        clean = word.strip()
        lang = mobapp.unicodedata2.script(clean[0])
        if lang == 'Katakana' or lang == 'Hiragana' or lang == 'Han':
            langCode = Language.objects.get(language_code='jp')
        else:
            langCode = Language.objects.get(language_code='en')
        return (clean, langCode)

    # finds or creates a dictionary entry, initializing it to inactive
    def _FindOrCreateEntry(self, word, lang, status, phonetic):
        try:
            entry = DictionaryWord.objects.get(word=word)
        except:
            # No entry, so create
            now = timezone.now()
            entry = DictionaryWord(word=word, language=lang, status=status,
                                   creationDate = now, modificationDate = now,
                                   phonetic = phonetic)

        return entry
        
    def _submitData(self, content):
        word = content.get(self.kSubmitWordKey, None)
        transList = content.get(self.kSubmitTranslationsKey, None)

        if word is None or transList is None or transList is None:
            return "insufficient or incorrect parameters", False

        inactive = Status.objects.get(name='pending') 
        wordPhonetic = content.get(self.kSubmitWordPhoneticKey)
        transDict = {}
        if type(transList) == type([]):
            # old style - only the translation, so set the phonetic to be None
            for translation in transList:
                transDict[translation] = None
        elif type(transList) == type({}):
            transDict = transList

        # Check and create all the words if necessary.
        phonetic = content.get(self.kSubmitWordPhoneticKey, None)
        clean, lang = self._CleanAndGetLang(word)
        mainEntry = self._FindOrCreateEntry(clean, lang, inactive, wordPhonetic)
        mainEntry.save()

        entryTrans = []
        for translation in transDict.keys():
            cleaned, lang = self._CleanAndGetLang(translation)
            phonetic = transDict[translation]
            transEntry = self._FindOrCreateEntry(cleaned, lang, 
                                                 inactive, phonetic)
            if transEntry:
                entryTrans.append(transEntry)
                transEntry.save()
                transEntry.translations.add(mainEntry) 
                transEntry.save()
        mainEntry.translations = entryTrans
        mainEntry.save()

        print('saved one word with %d translations' % len(transList))
        return {}, True
