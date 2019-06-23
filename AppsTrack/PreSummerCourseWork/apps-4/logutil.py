import inspect
from p1.helper import getLogger


def dumpLogs():
    parentModule = inspect.getmodule(inspect.currentframe().f_back)
    callerName = inspect.stack()[1].function
    logger = getLogger(parentModule.__name__)

    logger.info(f'info::entering method {parentModule.__name__}.{callerName}')

    logger.warning(f'warning::this is a log from {parentModule.__name__}.{callerName} at warning level')
    logger.error(f'error:this is a log from {parentModule.__name__}.{callerName} at error level')

    logger.info(f'info::exiting method {parentModule.__name__}.{callerName}')
