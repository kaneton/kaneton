#! /bin/bash

# spawn the registrar.
./registrar.py &

# spawn the scheduler.
./scheduler.py &

# set up the crontab.
crontab -r
crontab ../hooks/hooks.cron
