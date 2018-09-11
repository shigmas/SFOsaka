# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('mobapp', '0005_apphighlight'),
    ]

    operations = [
        migrations.CreateModel(
            name='Transportation',
            fields=[
                ('id', models.AutoField(verbose_name='ID', primary_key=True, serialize=False, auto_created=True)),
                ('name', models.CharField(max_length=512)),
                ('name_jp', models.CharField(max_length=512)),
                ('description', models.TextField(max_length=4096)),
                ('description_jp', models.TextField(max_length=4096)),
                ('shortDescription', models.CharField(max_length=1024)),
                ('shortDescription_jp', models.CharField(max_length=1024)),
                ('modificationDate', models.DateTimeField(auto_now_add=True)),
                ('imageUrl', models.CharField(max_length=512, null=True)),
                ('url', models.CharField(max_length=512, null=True)),
                ('order', models.PositiveSmallIntegerField()),
                ('detail', models.TextField(max_length=4096, null=True)),
                ('detail_jp', models.TextField(max_length=4096, null=True)),
                ('category', models.ForeignKey(to='mobapp.Category')),
                ('contact', models.ForeignKey(null=True, to='mobapp.ContactInfo')),
            ],
            options={
                'get_latest_by': 'modificationDate',
                'abstract': False,
            },
        ),
        migrations.AlterField(
            model_name='apphighlight',
            name='modificationDate',
            field=models.DateTimeField(auto_now_add=True),
        ),
        migrations.AlterField(
            model_name='partner',
            name='modificationDate',
            field=models.DateTimeField(auto_now_add=True),
        ),
        migrations.AlterField(
            model_name='performer',
            name='modificationDate',
            field=models.DateTimeField(auto_now_add=True),
        ),
    ]
