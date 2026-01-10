import time
import logging
from systemd.journal import JournalHandler
import signal
import sys
from logging.handlers import RotatingFileHandler
import os
import psutil

LOG_FILE = "/home/toxoplasma/my_service/info.log"
os.makedirs(os.path.dirname(LOG_FILE),exist_ok=True)
logger = logging.getLogger("my_service")
logger.setLevel(logging.INFO)

journal_handler = JournalHandler()
logger.addHandler(journal_handler)

file_handler = RotatingFileHandler(LOG_FILE,maxBytes=5*1024*1024,backupCount=3)
formatter = logging.Formatter('%(asctime)s[%(levelname)s] %(message)s')
file_handler.setFormatter(formatter)
logger.addHandler(file_handler)

logger.info("logging started")

running = True

def handle_sigterm(signum,frame):
	global running
	logger.info("Received SIGTERM, shutting down...")
	runnong = False
signal.signal(signal.SIGTERM,handle_sigterm)
signal.signal(signal.SIGINT,handle_sigterm)

while running:
	logger.info("service is running...")
	for proc in psutil.process_iter(['pid','name','cpu_percent','memory_percent']):
	    info = proc.info
	    logger.info(f"PID{info['pid']}| {info['name']} | CPU{info['cpu_percent']}% | MEM {info['memory_percent']:.2f}%")
	time.sleep(10)

logger.info("service stopped")
sys.exit(0)
