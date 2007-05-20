#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/julian/kaneton/kaneton/test/parse.py
#
# created       julian pidancet   [wed mar 21 20:01:38 2007]
# updated       julian pidancet   [wed mar 21 20:01:38 2007]
#

#
# ---------- imports ----------------------------------------------------------
#

import yaml

#
# ---------- functions --------------------------------------------------------
#

def			get_test_data():
  stream = file('test.yaml', 'r')
  data = yaml.load(stream)
  return data
    
def			get_test_name(testentry):
  try:
    name = testentry["name"]
  except KeyError:
    try:
      manager = testentry["manager"]
    except KeyError:
      raise YamlParser, 'Missing manager field in test description entry'
    try:
      category = testentry["category"]
    except KeyError:
      raise YamlParser, 'Missing category field in test description entry'
    try:
      number = testentry["number"]
    except KeyError:
      raise YamlParser, 'Missing number field in test description entry'
    name = manager + '_' + category + '_' + number
  return name

def			get_test_symbol(testentry):
  try:
    symbol = testentry["symbol"]
  except KeyError:
    name = get_test_name(testentry)
    symbol = 'check_' + name
  return symbol

def			get_source_list(data):
  source_list = []
  for i in data:
    try:
      source = i['source']
    except KeyError:
      source = ''
    if source != '':
      source_list.append(source)
  return source_list

