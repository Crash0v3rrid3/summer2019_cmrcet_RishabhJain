from django import template
# register zip
register = template.Library()

@register.filter(name='getargs_2')
def getargs(list1, list2):
    values = []
    for (x, y) in zip(list1, list2):
        if type(x) == str:
            x = list1[x]
            values.append((x, y))

    return values

@register.filter(name='concat')
def getargs(str1, str2):
    return str1.lower() + str2.lower()


