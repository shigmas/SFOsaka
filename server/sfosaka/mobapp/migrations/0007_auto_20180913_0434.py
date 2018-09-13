# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('mobapp', '0006_auto_20180906_1746'),
    ]

    operations = [
        migrations.AlterField(
            model_name='apphighlight',
            name='modificationDate',
            field=models.DateTimeField(auto_now=True),
        ),
        migrations.AlterField(
            model_name='partner',
            name='modificationDate',
            field=models.DateTimeField(auto_now=True),
        ),
        migrations.AlterField(
            model_name='performer',
            name='modificationDate',
            field=models.DateTimeField(auto_now=True),
        ),
        migrations.AlterField(
            model_name='transportation',
            name='modificationDate',
            field=models.DateTimeField(auto_now=True),
        ),
    ]
