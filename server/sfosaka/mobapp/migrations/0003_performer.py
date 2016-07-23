# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('mobapp', '0002_auto_20160714_1720'),
    ]

    operations = [
        migrations.CreateModel(
            name='Performer',
            fields=[
                ('id', models.AutoField(serialize=False, auto_created=True, primary_key=True, verbose_name='ID')),
                ('name', models.CharField(max_length=512)),
                ('name_jp', models.CharField(max_length=512)),
                ('description', models.CharField(max_length=4096)),
                ('description_jp', models.CharField(max_length=4096)),
                ('shortDescription', models.CharField(max_length=1024)),
                ('shortDescription_jp', models.CharField(max_length=1024)),
                ('modificationDate', models.DateTimeField()),
                ('imageUrl', models.CharField(max_length=512, null=True)),
                ('url', models.CharField(max_length=512, null=True)),
                ('startTime', models.DateTimeField()),
                ('endTime', models.DateTimeField()),
                ('category', models.ForeignKey(to='mobapp.Category')),
                ('contact', models.ForeignKey(to='mobapp.ContactInfo', null=True)),
                ('status', models.ForeignKey(to='mobapp.Status')),
            ],
            options={
                'abstract': False,
                'get_latest_by': 'modificationDate',
            },
        ),
    ]
