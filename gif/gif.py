import imageio
import os

def create_gif(input_folder, output_filename, duration=0.1):
    files = [os.path.join(input_folder, f) for f in sorted(os.listdir(input_folder)) if f.endswith('.ppm')]
    images = [imageio.imread(file) for file in files]
    imageio.mimsave(output_filename, images, duration=duration)

input_folder = '.'
output_filename = 'output.gif'
create_gif(input_folder, output_filename, duration=0.2)

