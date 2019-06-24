# IPL Clone Application

In order to restore database dump, please run the following -

~# mysql -u <db_username> -p

mysql> drop database if exists iplclone;

mysql> create database iplclone;

mysql> exit

~# mysql -u <db_username> -p < dump.sql

And edit the settings.py file to match your database credentials.
