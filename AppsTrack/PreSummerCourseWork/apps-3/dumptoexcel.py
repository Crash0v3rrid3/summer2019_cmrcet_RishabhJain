import click, openpyxl
from bs4 import BeautifulSoup

class ExcelWorkBook(click.ParamType):
    name = 'excel-workbook'

    def convert(self, value, param, ctx):
        ctx.filename = value
        try:
            wb = openpyxl.load_workbook(ctx.filename)  # open it
            sure = click.confirm(f'Are you sure you want to overwrite {ctx.filename}? ', default=False,
                                 abort=False)  # ask to overwrite

            if sure:  # If yes
                open(ctx.filename, 'w').close()  # delete file
                raise FileNotFoundError  # move on
            else:
                self.fail('Exiting', param, ctx)

        except FileNotFoundError:
            wb = openpyxl.Workbook()  # create new wb

        return wb

class HtmlSoup(click.ParamType):

    def convert(self, value, param, ctx):

        ctx.filename = value

        try:
            f = open(ctx.filename, 'rb')
        except:
            self.fail(f'{ctx.filename} doesn\'t exist', param, ctx)

        soup = BeautifulSoup(f)
        f.close()
        return soup

def writeColumnNames(source, destination):
    tableRow = source.html.table.tr
    ws = destination.active

    for column, th in enumerate(tuple(tableRow.find_all('th'))[1: ], start = 1):
        ws.cell(row = 1, column = column).value = str(th.string)

def writeData(source, destination):
    table = source.html.table
    ws = destination.active

    for row, tableRow in enumerate(tuple(table.find_all('tr'))[1: ], start = 2):
        for column, td in enumerate(tuple(tableRow.find_all('td'))[1:], start = 1):
            ws.cell(row = row, column = column).value = str(td.string)

@click.command()
@click.argument('source', type = HtmlSoup())
@click.argument('destination', type = ExcelWorkBook())
@click.pass_context
def start(ctx, source, destination):
    writeColumnNames(source, destination)
    writeData(source, destination)
    destination.save(ctx.filename)

if __name__ == '__main__':
    start()