#!/usr/bin/env python3
import logging
from systemd.journal import JournalHandler
from logging.handlers import RotatingFileHandler
import os
import psutil

LOG_FILE = "/home/toxoplasma/my_service/info.log"
os.makedirs(os.path.dirname(LOG_FILE), exist_ok=True)

logger = logging.getLogger("monitor")
logger.setLevel(logging.INFO)


journal_handler = JournalHandler()
logger.addHandler(journal_handler)

file_handler = RotatingFileHandler(LOG_FILE, maxBytes=5*1024*1024, backupCount=3)
formatter = logging.Formatter('%(asctime)s [%(levelname)s] %(message)s')
file_handler.setFormatter(formatter)
logger.addHandler(file_handler)


logger.info("cron run started")

for proc in psutil.process_iter(['pid', 'name', 'cpu_percent', 'memory_percent']):
    info = proc.info
    logger.info(
        f"PID {info['pid']} | {info['name']} | "
        f"CPU {info['cpu_percent']}% | MEM {info['memory_percent']:.2f}%"
    )
    for handler in logger.handlers:
        handler.flush()

logger.info("cronjob run finished")
for handler in logger.handlers:
    handler.flush()
