import os


list_of_paths = []

#------------------------------------------------------------------------------------------#

# Metal Textures

metal_textures_dir = os.listdir("../Textures/T_Metal")

for metal_textures in metal_textures_dir:
    list_of_paths.append("Textures/T_Metal/" + metal_textures +'\n')

#------------------------------------------------------------------------------------------#

# Wood Textures

wood_textures_dir = os.listdir("../Textures/T_Wood")

for wood_textures in wood_textures_dir:
    list_of_paths.append("Textures/T_Wood/" + wood_textures + '\n')

#------------------------------------------------------------------------------------------#

# Rock Textures

rock_textures_dir = os.listdir("../Textures/T_Rock")

for rock_texture in rock_textures_dir:
    list_of_paths.append("Textures/T_Rock/" + rock_texture + '\n')

#------------------------------------------------------------------------------------------#

# Brick Textures

brick_textures_dir = os.listdir("../Textures/T_Brick")

for brick_texture in brick_textures_dir:
    list_of_paths.append("Textures/T_Brick/" + brick_texture + '\n')

#------------------------------------------------------------------------------------------#

# Fabric Textures

fabric_textures_dir = os.listdir("../Textures/T_Fabric")

for fabric_texture in fabric_textures_dir:
    list_of_paths.append("Textures/T_Fabric/" + fabric_texture + '\n')

#------------------------------------------------------------------------------------------#

# Write into txt file

with open("textures_list.txt", "a") as my_file:
    for path in list_of_paths:
        my_file.write(path)

#------------------------------------------------------------------------------------------#
