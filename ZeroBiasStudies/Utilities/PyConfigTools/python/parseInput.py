import sys

def parseInput(inputFields,requiredFields = ()):

    class options: pass
    for item in sys.argv:
        option = item.split('=')[0].strip()
        if option in inputFields:
            value = item.split('=')[1].strip()
            if value in ('true','True','yes','Yes'): value = True
            elif value in ('false','False','no','No'): value = False

            setattr(options,option,value)

    for item in requiredFields:
        if not hasattr(options,item):
            raise RuntimeError,'Need to set "%s"' % item

    return options
