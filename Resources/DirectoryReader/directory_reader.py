import os


list_of_paths = []

#------------------------------------------------------------------------------------------#

metal_textures_dir = os.listdir("../Textures/MetalTextures")

for metal_textures in metal_textures_dir:
    list_of_paths.append("Textures/MetalTextures/" + metal_textures +'\n')

#------------------------------------------------------------------------------------------#

wood_textures_dir = os.listdir("../Textures/WoodTextures")

for wood_textures in wood_textures_dir:
    list_of_paths.append("Textures/WoodTextures/" + wood_textures + '\n')

#------------------------------------------------------------------------------------------#

brick_textures_dir = os.listdir("../Textures/BrickTextures")

for brick_texture in brick_textures_dir:
    list_of_paths.append("Textures/BrickTextures/" + brick_texture + '\n')

#------------------------------------------------------------------------------------------#

with open("textures_list.txt", "a") as my_file:
    for path in list_of_paths:
        my_file.write(path)

#------------------------------------------------------------------------------------------#
