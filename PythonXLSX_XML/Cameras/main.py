import xlrd
from lxml import etree


def configs(etreeObject, etreeparent, name, values):
    etreeitem = etreeObject.SubElement(etreeparent, name)
    etreeitem.text = values
    return

def main():
    root = etree.Element("CCTV")
    root.attrib['StreamType'] = '0'
    root.attrib['Name'] = 'CCTV监控'

    ExcelFile = xlrd.open_workbook('D:\\Cameras.xlsx')
    sheet = ExcelFile.sheet_by_index(0)
    row_num = sheet.nrows
    col_num = sheet.ncols
    print(sheet.nrows)
    print(sheet.ncols)
    for i in range(1, row_num):
        areaname = sheet.cell_value(i, 0)
        if areaname.strip() != '':
            areaElement = etree.SubElement(root, 'Group')
            areaElement.attrib['CCTVType'] = '1'
            configs(etree, areaElement, 'name', areaname)

        positionElement = etree.SubElement(areaElement, 'Group')
        positionElement.attrib['CCTVType'] = '1'

        configs(etree, positionElement, 'name', sheet.cell_value(i, 1))
        configs(etree, positionElement, 'ip', sheet.cell_value(i, 2))
        configs(etree, positionElement, 'Port', '%d' % sheet.cell_value(i, 3))
        configs(etree, positionElement, 'UserName', sheet.cell_value(i, 4))
        configs(etree, positionElement, 'Password', sheet.cell_value(i, 5))
        devicetypeElement = etree.SubElement(positionElement, 'DeviceType')
        deviceType = sheet.cell_value(i, 6)
        if deviceType == '海康':
            devicetypeElement.text = '1'
        else:
            devicetypeElement.text = '2'
        configs(etree, positionElement, 'serialno', '2')
        configs(etree, positionElement, 'baudrate', '4')
        configs(etree, positionElement, 'databits', '8')
        configs(etree, positionElement, 'stopbits', '1')
        configs(etree, positionElement, 'parity', '3')
        channelElement = etree.SubElement(positionElement, 'channel')

        configs(etree, channelElement, 'id', '%d' % sheet.cell_value(i, 7))
        configs(etree, channelElement, 'name', sheet.cell_value(i, 1))
        configs(etree, channelElement, 'ifControl', '1')
        configs(etree, channelElement, 'Longitude', '%s' % sheet.cell_value(i, 8))
        configs(etree, channelElement, 'Latitude', '%s' % sheet.cell_value(i, 9))
        configs(etree, channelElement, 'PlatFormID', '%d' % i)
        configs(etree, channelElement, 'height', '%s' % sheet.cell_value(i, 10))
        configs(etree, channelElement, 'focus', '5')
        configs(etree, channelElement, 'focustime', '9000')
        configs(etree, channelElement, 'leveloffset', '188.000000')
        configs(etree, channelElement, 'apkoffset', '7.500000')
        configs(etree, channelElement, 'apklevoffset', '0.000000')
        configs(etree, channelElement, 'apkzazi', '0.000000')
        configs(etree, channelElement, 'lineno', '0')
        configs(etree, channelElement, 'transno', '0')
        configs(etree, channelElement, 'picqty', '1')
        configs(etree, channelElement, 'picsize', '0')
        configs(etree, channelElement, 'nightvision', '1')
        configs(etree, channelElement, 'opposite', '0')
        configs(etree, channelElement, 'autofocus', '1')
        configs(etree, channelElement, 'PhotoType', '1')
        configs(etree, channelElement, 'SCreenOccuptionRatio', '4')
        configs(etree, channelElement, 'DistanceCoefficient1', '1.310000')
        configs(etree, channelElement, 'DistanceCoefficient2', '0.910000')
        configs(etree, channelElement, 'PhotoNearSCreenRatio', '7')
        configs(etree, channelElement, 'PhotoNearDisCoeff1', '2.100000')
        configs(etree, channelElement, 'PhotoNearDisCoeff2', '2.200000')
        configs(etree, channelElement, 'PhotoFarSCreenRatio', '5')
        configs(etree, channelElement, 'PhotoFarDisCoeff1', '2.500000')
        configs(etree, channelElement, 'PhotoFarDisCoeff2', '2.600000')

    dataxml = etree.tostring(root, pretty_print=True, encoding="utf-8", method="xml", xml_declaration=True,
                             standalone=True).decode('utf-8')
    xmlFileOut = 'D:\\cameras.xml'

    with open(xmlFileOut, "w", encoding='utf-8') as f:
        f.write(dataxml)
        f.close()
    return
main()