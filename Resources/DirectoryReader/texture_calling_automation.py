import os


list_of_paths = []

i1 = 0
i2 = 0

transparent_textures_sub_dir = os.listdir("../Textures/TransparentTextures")

for transparent_texture in transparent_textures_sub_dir:
    i2 += 1
    list_of_paths.append("Texture transparent" + str(i2) + '(environment::ResourcePath("' + "Textures/TransparentTextures/" + transparent_texture + '"));' + '\n')

#------------------------------------------------------------------------------------------#

metal_textures_dir = os.listdir("../Textures/MetalTextures")

for metal_textures in metal_textures_dir:
    i1 += 1
    list_of_paths.append("Texture texture" + str(i1) + '(environment::ResourcePath("' + "Textures/MetalTextures/" + metal_textures + '"));' + '\n')

#------------------------------------------------------------------------------------------#

wood_textures_dir = os.listdir("../Textures/WoodTextures")

for wood_textures in wood_textures_dir:
    i1 += 1
    list_of_paths.append("Texture texture" + str(i1) + '(environment::ResourcePath("' + "Textures/WoodTextures/" + wood_textures + '"));' + '\n')

#------------------------------------------------------------------------------------------#

brick_textures_dir = os.listdir("../Textures/BrickTextures")

for brick_texture in brick_textures_dir:
    i1 += 1
    list_of_paths.append("Texture texture" + str(i1) + '(environment::ResourcePath("' + "Textures/BrickTextures/" + brick_texture + '"));' + '\n')

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------#

with open("texture_generating_calls.txt", "a") as my_file:
    for path in list_of_paths:
        my_file.write(path)

