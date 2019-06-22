from django.template.defaulttags import register


@register.filter
def get_item(dictionary, key):
    return dictionary.get(key)

@register.filter
def dict_items(dictionary):
    return dictionary.items()

@register.filter
def dict_keys(dictionary):
    return dictionary.keys()

@register.filter
def getattr_or_none(obj, key):
    try:
        return getattr(obj, key)
    except:
        return None
