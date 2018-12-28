# -*- coding: utf-8 -*-
from lxml import etree

data = etree.Element("data")
#1 interface_version
interface_version_txt = '5'
interface_version = etree.SubElement(data, 'interface_version')
interface_version.text = interface_version_txt
#2 site
site_txt = 'www.xxx.com'
site = etree.SubElement(data, 'site')
site.text = site_txt
#5 app
app = etree.SubElement(data, 'app')
#6 title
title_txt = '工地要有'
#title_txt = etree.CDATA(title_txt)
title = etree.SubElement(app, 'title')
title.text = title_txt
print(title.text)
#7 appid
appid = etree.SubElement(app, 'appid')
appid.text = '%s' % 2
dataxml = etree.tostring(data, pretty_print=True, encoding="utf-8", method="xml", xml_declaration=True, standalone=True).decode('utf-8')
print(dataxml)
xmlFileOut = 'D:\\test.xml'

with open(xmlFileOut, "w", encoding='utf-8') as f:
    f.write(dataxml)
    f.close()
