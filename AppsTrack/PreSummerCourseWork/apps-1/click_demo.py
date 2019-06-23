import click

@click.group()
def demo():
    pass


@demo.command(name='secret')
@click.option('-u', '--username',
              required=True,
              help='The username whose secret is desired',
              type=str,
              prompt=True)
@click.option('-p', '--password',
              required=True,
              help='The password whose secret is desired',
              type=str,
              prompt=True,
              hide_input=True,
              confirmation_prompt=True)
def secret(username, password):
    click.echo(username)
    click.echo(password)
    click.echo(f'The secret for {username} is \033[32m42\033[32m')


if __name__ == '__main__':
    demo()
