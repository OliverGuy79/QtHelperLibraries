import os

# folder path
dir_path = r'/home/olivier/Documents/QtWorkspace/CuteSimpleRestClient/QmlComponentLibrary/Icons/media/svg/Material/Rounded/'

# list to store files
res = []

# Iterate directory
for path in os.listdir(dir_path):
    # check if current path is a file
    if os.path.isfile(os.path.join(dir_path, path)):
        os.rename(dir_path+path, dir_path+path[0: path.find('_FILL')]+'.svg')
        res.append(path)
print(res)


