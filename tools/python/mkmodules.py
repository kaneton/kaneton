from	 distutils.core import setup, Extension

module1 = Extension('kserial', sources = ['kserialmodules.c'])

setup (names = 'kaneton serial',
		version = '1.0',
		description = 'serial encapsulated communication for kaneton OS',
		ext_modules = [module1])
