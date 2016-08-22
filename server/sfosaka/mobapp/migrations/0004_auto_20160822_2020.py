# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('mobapp', '0003_performer'),
    ]

    operations = [
        migrations.AlterField(
            model_name='partner',
            name='description',
            field=models.TextField(max_length=4096),
        ),
        migrations.AlterField(
            model_name='partner',
            name='description_jp',
            field=models.TextField(max_length=4096),
        ),
        migrations.AlterField(
            model_name='performer',
            name='description',
            field=models.TextField(max_length=4096),
        ),
        migrations.AlterField(
            model_name='performer',
            name='description_jp',
            field=models.TextField(max_length=4096),
        ),
    ]
