import xlrd

ExcelFile=xlrd.open_workbook('D:\\Cameras.xlsx')
sheet=ExcelFile.sheet_by_index(0)
row_num = sheet.nrows
col_num = sheet.ncols
print(sheet.nrows)
print(sheet.ncols)
for i in range(1,4):
    for j in range(col_num):
        print(sheet.cell_value(i,j))