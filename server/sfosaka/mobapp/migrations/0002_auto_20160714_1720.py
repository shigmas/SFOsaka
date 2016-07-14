# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('mobapp', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='ContactInfo',
            fields=[
                ('id', models.AutoField(serialize=False, verbose_name='ID', auto_created=True, primary_key=True)),
                ('street_number', models.CharField(max_length=1024)),
                ('city', models.CharField(max_length=512)),
                ('state', models.CharField(max_length=2)),
                ('postal', models.CharField(max_length=128)),
                ('country', models.CharField(max_length=512)),
                ('phone', models.CharField(max_length=64)),
            ],
        ),
        migrations.AddField(
            model_name='dictionaryword',
            name='phonetic',
            field=models.CharField(null=True, max_length=128),
        ),
        migrations.AddField(
            model_name='partner',
            name='url',
            field=models.CharField(null=True, max_length=512),
        ),
        migrations.AlterField(
            model_name='partner',
            name='imageUrl',
            field=models.CharField(null=True, max_length=512),
        ),
        migrations.AddField(
            model_name='partner',
            name='contact',
            field=models.ForeignKey(null=True, to='mobapp.ContactInfo'),
        ),
    ]
