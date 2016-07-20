from django.contrib import admin

from django.utils import timezone

from mobapp import models

# Register your models here.
admin.site.register(models.ContactInfo)
admin.site.register(models.Category)
admin.site.register(models.Partner)
admin.site.register(models.Status)
admin.site.register(models.Language)

#@admin.register(models.DictionaryWord)
class DictionaryWordAdmin(admin.ModelAdmin):
    list_display = ('__str__', 'word','language','status', 'translation_as_word')
    list_filter_fields = ('status')
    actions = ('approve_words','reject_words')

    def _setStatusOnQuerySet(self, status, queryset):
        nowDate = timezone.now()
        for word in queryset:
            word.status = status
            word.modificationDate = nowDate
            word.save()

    def approve_words(self, request, queryset):
        """
        Sets the selected words as approved
        """
        activeStatus = models.Status.objects.get(name='active')
        self._setStatusOnQuerySet(activeStatus, queryset);

    def reject_words(self, request, queryset):
        """
        Sets the selected words as rejected
        """
        dormantStatus = models.Status.objects.get(name='dormant')
        self._setStatusOnQuerySet(dormantStatus, queryset);

admin.site.register(models.DictionaryWord, DictionaryWordAdmin)

