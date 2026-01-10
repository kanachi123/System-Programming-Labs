#!/usr/bin/env python3 

# #типо когда так пишешь система позволяет запустить неявно без написания python3,
# а как обычный исполняемый файл(хотя это не одно и то же)

import logging """ это типо для логирования """
""" это для логирования в журнал systemd 
чтобы потом можно было смотреть через journalctl """
from systemd.journal import JournalHandler 
""" это для логирования в файл с ротацией,
чтобы файл не разрастался до гигабайтов и не засорял твой дорогой ssd и ты копаля бы в нем потом """
from logging.handlers import RotatingFileHandler
""" это для работы с операционной системой,
в этом контексте для работы с файлами,папки создание... """
import os

""" эта библиотека позволяет работать с процессами и смотреть их параметры 
через питон, типа как ps в линуксе ,сколько жрет твой туенядец процесс в какое время суток"""

import psutil

""" Настройка логгера для записи в журнал systemd и в файл с ротацией"""
LOG_FILE = "/home/toxoplasma/my_service/info.log"
""" создаем папку если ее нет """
os.makedirs(os.path.dirname(LOG_FILE), exist_ok=True)

#создаем логгер
logger = logging.getLogger("monitor")
# устанавливаем уровень логирования на INFO
logger.setLevel(logging.INFO)

""" создаем обработчик для журналов systemd """
journal_handler = JournalHandler()
# добавляем обработчик в логгер
logger.addHandler(journal_handler)

""" создаем обработчик для файлов с ротацией максимум 5 мегабайт на файл и 3 лога,
смысл ротации в том что эти 3 максимальное количество самое старое удалеятся и заменяется новой """
file_handler = RotatingFileHandler(LOG_FILE, maxBytes=5*1024*1024, backupCount=3)
# устанавливаем формат логов,то есть как они будут выглядеть: время,уровень,сообщение
formatter = logging.Formatter('%(asctime)s [%(levelname)s] %(message)s')
file_handler.setFormatter(formatter)
logger.addHandler(file_handler)

#для красоты
logger.info("cron run started")

""" через  цикл проходим по всем процессам в системе и
 логируем их PID,имя,процент CPU и процент памяти """
for proc in psutil.process_iter(['pid', 'name', 'cpu_percent', 'memory_percent']):
    info = proc.info
    logger.info(
        f"PID {info['pid']} | {info['name']} | "
        f"CPU {info['cpu_percent']}% | MEM {info['memory_percent']:.2f}%"
    )
    # обязательно сбрасываем буферы логов чтобы они записались на диск и в журнал
    for handler in logger.handlers:
        # сброс буфера
        handler.flush()
#для красоты
logger.info("cronjob run finished")
# сбрасываем буферы логов в конце скрипта,чтобы все логи точно записались
for handler in logger.handlers:
    handler.flush()
