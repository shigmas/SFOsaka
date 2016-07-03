# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Category',
            fields=[
                ('id', models.AutoField(verbose_name='ID', primary_key=True, serialize=False, auto_created=True)),
                ('name', models.CharField(max_length=64)),
                ('description', models.CharField(max_length=1024)),
            ],
        ),
        migrations.CreateModel(
            name='DictionaryWord',
            fields=[
                ('id', models.AutoField(verbose_name='ID', primary_key=True, serialize=False, auto_created=True)),
                ('word', models.CharField(max_length=128)),
                ('creationDate', models.DateTimeField()),
                ('modificationDate', models.DateTimeField()),
            ],
            options={
                'get_latest_by': 'modificationDate',
            },
        ),
        migrations.CreateModel(
            name='Language',
            fields=[
                ('id', models.AutoField(verbose_name='ID', primary_key=True, serialize=False, auto_created=True)),
                ('name', models.CharField(max_length=64)),
                ('region', models.CharField(max_length=2)),
                ('language_code', models.CharField(max_length=2)),
            ],
        ),
        migrations.CreateModel(
            name='Partner',
            fields=[
                ('id', models.AutoField(verbose_name='ID', primary_key=True, serialize=False, auto_created=True)),
                ('name', models.CharField(max_length=512)),
                ('name_jp', models.CharField(max_length=512)),
                ('latitude', models.FloatField()),
                ('longitude', models.FloatField()),
                ('description', models.CharField(max_length=4096)),
                ('description_jp', models.CharField(max_length=4096)),
                ('shortDescription', models.CharField(max_length=1024)),
                ('shortDescription_jp', models.CharField(max_length=1024)),
                ('modificationDate', models.DateTimeField()),
                ('imageUrl', models.CharField(max_length=512)),
                ('category', models.ForeignKey(to='mobapp.Category')),
            ],
            options={
                'get_latest_by': 'modificationDate',
            },
        ),
        migrations.CreateModel(
            name='Status',
            fields=[
                ('id', models.AutoField(verbose_name='ID', primary_key=True, serialize=False, auto_created=True)),
                ('name', models.CharField(max_length=64)),
            ],
        ),
        migrations.AddField(
            model_name='dictionaryword',
            name='language',
            field=models.ForeignKey(to='mobapp.Language'),
        ),
        migrations.AddField(
            model_name='dictionaryword',
            name='status',
            field=models.ForeignKey(to='mobapp.Status'),
        ),
        migrations.AddField(
            model_name='dictionaryword',
            name='translations',
            field=models.ManyToManyField(to='mobapp.DictionaryWord', blank=True, related_name='translations_rel_+'),
        ),
    ]
