import click, openpyxl
from copy import copy

class ExcelWorkBook(click.ParamType): # custom type
    name = 'excel-work-book'

    def __init__(self, wbType):
        self.wbType = wbType

    def convert(self, value, param, ctx):
        wbName = value

        if self.wbType == 'source': # check if source wb

            try:
                wb = openpyxl.load_workbook(wbName) # open it
            except FileNotFoundError:
                self.fail(f'Could not open file {self.wbName}') # if doesn't exist fail

        elif self.wbType == 'destination':  # check if destination file
            ctx.filename = wbName
            try:
                wb = openpyxl.load_workbook(wbName) # open it
                sure = click.confirm(f'Are you sure you want to overwrite {wbName}? ', default=False, abort=False)  #ask to overwrite

                if sure:    # If yes
                    open(wbName, 'w').close() # delete file
                    raise FileNotFoundError # move on
                else:
                    self.fail('Exiting', param, ctx)

            except FileNotFoundError:
                wb = openpyxl.Workbook()    # create new wb

        return wb



def copySheets(source, dest, capitalize, preservestyles):
    for sheetnumber, sheetname in enumerate(source.sheetnames):
        if sheetname not in dest.sheetnames:
            dest.create_sheet(sheetname)

        dest.active, source.active = dest.sheetnames.index(sheetname), sheetnumber

        destWS = dest.active
        sourceWS = source.active

        destMaxRow = 0 if destWS.max_row == 1 else destWS.max_row
        sourceMaxRow, sourceMaxColumn = sourceWS.max_row, sourceWS.max_column

        for row in range(1, sourceMaxRow + 1):
            for column in range(1, sourceMaxColumn + 1):

                destCell = destWS.cell(row = destMaxRow + row, column = column)
                sourceCell = sourceWS.cell(row = row, column = column)

                destCell.value = sourceCell.value.upper() if capitalize else sourceCell.value

                if preservestyles:          # copy cell styles
                    destCell.font = copy(sourceCell.font)
                    destCell.fill = copy(sourceCell.fill)
                    destCell.border = copy(sourceCell.border)
                    destCell.alignment = copy(sourceCell.alignment)
                    destCell.number_format = copy(sourceCell.number_format)
                    destCell.protection = copy(sourceCell.protection)

        if preservestyles:      # copy worksheet styles
            destWS.sheet_format = copy(sourceWS.sheet_format)
            destWS.sheet_properties = copy(sourceWS.sheet_properties)
            destWS.merged_cells = copy(sourceWS.merged_cells)
            destWS.page_margins = copy(sourceWS.page_margins)
            destWS.page_setup = copy(sourceWS.page_setup)
            destWS.print_options = copy(sourceWS.print_options)

            for attr in ('row_dimensions', 'column_dimensions'):
                src = getattr(sourceWS, attr)
                target = getattr(destWS, attr)
                for key, dim in src.items():
                    target[key] = copy(dim)
                    target[key].worksheet = target


@click.command(help='A utility to transform excel sheets')
@click.option('-c', '--capatilize', is_flag=True, default=False)
@click.option('-p', '--preservestyles', is_flag=True, default=False)
@click.argument('source_excel', nargs=-1, type = ExcelWorkBook('source'))
@click.argument('dest_excel', nargs=1, type = ExcelWorkBook('destination'))
@click.pass_context
def transform(ctx, capatilize, preservestyles, source_excel, dest_excel):
    # get rid of unwanted sheets
    for useless in dest_excel.sheetnames:
        uselessSheet = dest_excel.get_sheet_by_name(useless)
        dest_excel.remove(uselessSheet)

    # multiple workbooks given
    for sourceWorkBook in source_excel:
        # last worksheet is extra and useless
        sourceWorkBook.remove(sourceWorkBook.get_sheet_by_name(sourceWorkBook.sheetnames[-1]))
        copySheets(sourceWorkBook, dest_excel, capatilize, preservestyles)
    dest_excel.save(ctx.filename)

if __name__ == '__main__':
    transform()