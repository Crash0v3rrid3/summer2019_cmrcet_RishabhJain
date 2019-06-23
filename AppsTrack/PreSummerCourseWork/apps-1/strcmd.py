import click, string

@click.group(help='\tSupports some string commands from command line')
@click.option(
    '-rd/-nrd',
    '--removedigits/--noremovedigits',
    help='remove digits from input'
)
@click.pass_context
def parseArguments(ctx, removedigits):
    ctx.obj = {'removedigits': removedigits}

@parseArguments.command(
    name='concat',
    short_help='concatenates passed in strings with delimiter',
    help='\tpass one or more strings, concat them with delimiter  and print them out'
)
@click.option(
    '-d',
    '--delimiter',
    default=":",
    help="defaults to :"
)
@click.argument('tokens', nargs = -1)
@click.pass_context
def concat(ctx, delimiter, tokens):
    if ctx.obj['removedigits']:
        tokens = tuple(map(removeDigits, tokens))
    click.echo(delimiter.join(tokens))

@parseArguments.command(
    name='lower',
    help='converts the word to lower case'
)
@click.argument('token')
@click.pass_context
def lower(ctx, token):
    if ctx.obj['removedigits']:
        token = removeDigits(token)
    click.echo(token.lower())

@parseArguments.command(
    name='upper',
    help='converts the word to upper case'
)
@click.argument('token')
@click.pass_context
def upper(ctx, token):
    if ctx.obj['removedigits']:
        token = removeDigits(token)
    click.echo(token.upper())

def removeDigits(myString):
    return ''.join(list(filter(lambda x: x not in string.digits, myString)))

if __name__ == '__main__':
    parseArguments()