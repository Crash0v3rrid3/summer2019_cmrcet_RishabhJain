import logging
import logging.config

configuration2 = {
    'version': 1,
    'disable_existing_loggers': False,
    'loggers': {
        '': {
            'handlers': ['console', 'mainlog'],
            'level': "DEBUG",
        },
        'p1.p2': {
            'handlers': ['p2log', 'console'],
            'level': "DEBUG",
            'propagate': False
        },
        'p1': {
            'handlers': ['p1log', 'console'],
            'level': "DEBUG",
            'propagate': False
        },
    },
    'handlers': {
        'console': {
            'level': 'WARNING',
            'class': 'logging.StreamHandler',
            'stream': 'ext://sys.stdout'
        },
        'p2log': {
            'class': 'logging.FileHandler',
            'filename': 'p2.log'
        },

        'p1log': {
            'class': 'logging.FileHandler',
            'filename': 'p1.log'
        },
        'mainlog': {
            'class': 'logging.FileHandler',
            'filename': 'main.log'
        }
    }
}


def configure_logger(name):
    global configuration1, configuration2

    logging.config.dictConfig(configuration2)
    return logging.getLogger(name)

def getLogger(name):
    logger = configure_logger(name)

    return logger